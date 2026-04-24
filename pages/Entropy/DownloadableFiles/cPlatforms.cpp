#include "cPlatforms.h"

cPlatforms::cPlatforms(const int& _uuid, const int& _asset_ID, const std::string& _name, const std::weak_ptr<cScene> _scene) 
	: cModelNode(_uuid, _asset_ID),
	tg::iInputListener{ tg::Input::eType::kAll, 2048 }
{ 
	setName(_name);
	GetPoints(_scene);

	tg::Input::addListener(this);
	platform.diff = (platform.m_prev_pos - platform.m_next_pos).length();
} //cPlatforms

//////////////////////////////////////////////////////////////////////////

cPlatforms::~cPlatforms()
{
	tg::Input::removeListener(this);
} //~cPlatforms

bool cPlatforms::onInputEvent(const uint32_t _type, const tg::Input::sEvent& _event)
{
	switch (_type)
	{
	case (tg::Input::eType::kKeyDown):
		if (_event.keyboard.repeat == 0)
		{
			switch (_event.keyboard.virtual_key)
			{
			case(VK_F9):
				platform.m_is_active ^= true;
				break;
			}
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////

void cPlatforms::update(const float& _delta_time)
{
	for (auto& pos : positions)
	{
		tg::cDebugLines::getInstance().addSphere3D(tg::cSphere3f(pos, 0.5f), tg::Color::kBlue);
	}

	if (indexx >= 10)
	{
		checkForPlayer(player_line);
	}
	indexx++;

	checkForOrb();

	stateMachine(platform.m_current_state, _delta_time);

	platform.trigger->render3d();

	if (platform.debug_mode) {
		tg::cDebugLines::getInstance().addSphere3D(tg::cSphere3f(getWorldPosition(), 0.5f), tg::Color::kDeepPink);
		tg::cDebugLines::getInstance().addSphere3D(tg::cSphere3f(platform.m_next_pos, 0.75f), tg::Color::kGreen);
		tg::cDebugLines::getInstance().addSphere3D(tg::cSphere3f(platform.m_prev_pos, 0.65f), tg::Color::kAqua);
	}
} //update

//////////////////////////////////////////////////////////////////////////

void cPlatforms::checkForPlayer(tg::cLine3f _player_line)
{
	if (platform.trigger->doesIntersect(_player_line))
	{
		if (!is_player_child)
		{
			addChild(player_ptr);
			printf("CHILDED\n");
		}
		is_player_child = true;
	}
	else
	{
		if (is_player_child)
		{
			removeChild(player_ptr->getID());
			printf("ORPHAN\n");
		}
		is_player_child = false;
	}
}

void cPlatforms::checkForOrb()
{
	for (auto orb : orb_handler_ptr->orbs)
	{
		if (orb->is_active && platform.trigger->doesIntersect(orb->orb_line) && !has_changed)
		{
			if (!platform.m_is_active)
			{
				printf("CHANGED TO TRUE");
				orb->is_active = false;
				//addChild(orb);
				platform.m_is_active = true;
				has_changed = true;
			}
			else if (platform.m_is_active)
			{
				printf("CHANGED TO FALSE");
				orb->is_active = false;

				platform.m_is_active = false;
				has_changed = true;
			}
		}
	}
	has_changed = false;
}

void cPlatforms::updatePlayerLine(tg::cLine3f& _player_line)
{
	player_line = _player_line;
}

void cPlatforms::updateOrbsLine(std::vector<tg::cLine3f>& _orb_line)
{
	orb_lines = _orb_line;
}

void cPlatforms::addTrigger(const std::weak_ptr<cScene> _scene)
{
	platform.trigger = std::make_shared<cTriggerNode>(_scene.lock()->getUniqueNodeID());

	addChild(platform.trigger);
	platform.trigger->setLocalScale({ 4.0f, 0.8f, 4.0f });
}

void cPlatforms::GetPoints(const std::weak_ptr<cScene> _scene) {
	//gets all the points in the scene
	auto searchForPlatformsPoints = [](const std::shared_ptr<iNode> _node) {
		if (_node->hasTag("is_point"))
		{
			_node->setHidden(true);
			return true;
		}
		return false;
		};

	auto points = _scene.lock()->searchAllNodes(searchForPlatformsPoints);

	std::sort(points.begin(), points.end(), [](std::shared_ptr<iNode> _a, std::shared_ptr<iNode> _b) {
		return std::stoi(_a->getName()) < std::stoi(_b->getName());
		});

	for (auto& point : points)
	{
		if (point->hasTag(getName()))
		{
			auto pos = point->getWorldPosition();
			positions.push_back(pos);
		}
	}

	//sets up platforms start pos

	//sets up points
	platform.m_prev_pos = positions[0];
	platform.m_next_pos = positions[1];

	platform.m_prev_point_index = 0;
	platform.m_next_point_index = 1;

	//sets up index
	platform.m_max_size_of_index = static_cast<int>(positions.size()) - 1;
	platform.diff = (platform.m_prev_pos - platform.m_next_pos).length();
} //GetPoints

//////////////////////////////////////////////////////////////////////////

void cPlatforms::changeIndex(const int& _flow_of_time)
{
	platform.m_prev_point_index = std::clamp(platform.m_prev_point_index += _flow_of_time, 0, platform.m_max_size_of_index - 1);
	platform.m_prev_pos         = positions[std::clamp(platform.m_prev_point_index, 0, platform.m_max_size_of_index)];

	platform.m_next_point_index = std::clamp(platform.m_next_point_index += _flow_of_time, 1, platform.m_max_size_of_index);
	platform.m_next_pos         = positions[std::clamp(platform.m_next_point_index, 1, platform.m_max_size_of_index)];

	platform.diff = (platform.m_prev_pos - platform.m_next_pos).length();
}

void cPlatforms::stateMachine(const sPlatforms::state& _state, const float& _delta_time)
{
	int   time_flow  = platform.m_is_active ? 1 : -1;
	float time_max   = platform.m_is_active ? 1.0f : 0.0f;
	float time_reset = platform.m_is_active ? 0.0f : 1.0f;

	switch (_state)
	{
	case(sPlatforms::state::isStart):
		platform.m_time = 0.0f;
		if (platform.m_is_active)
		{
			platform.m_current_state = sPlatforms::state::isMoving;
		}
		break;

	case(sPlatforms::state::isMoving):
		platform.m_time = std::clamp(platform.m_time += (std::clamp(_delta_time, 0.0f, 1.0f) * time_flow * platform.m_point_travel_time) / platform.diff, 0.0f, 1.0f);

		if (platform.m_is_active ? (platform.m_time >= time_max) : (platform.m_time <= time_max))
		{
			if (platform.m_is_active ? (platform.m_next_point_index == platform.m_max_size_of_index) : (platform.m_prev_point_index == 0))
			{
				platform.m_is_active ? (platform.m_current_state = sPlatforms::state::isEnd) : (platform.m_current_state = sPlatforms::state::isStart);
				break;
			}
			platform.m_time = time_reset;
			changeIndex(time_flow);
		}
		break;

	case(sPlatforms::state::isEnd):
		platform.m_time = 1.0f;
		if (!platform.m_is_active)
		{
			platform.m_current_state = sPlatforms::state::isMoving;
		}
		break;
	}

	lerpBetweenPoints();
}

void cPlatforms::lerpBetweenPoints() {
	{ //lerp forwards in the vector
		tg::cMatrix4x4f position = tg::cMatrix4x4f{}.translate(tg::Lerp::linear(platform.m_prev_pos, platform.m_next_pos, platform.m_time));
		setMatrix(position, true, true, false);
	}

	

} //lerpBetweenPoints

//////////////////////////////////////////////////////////////////////////