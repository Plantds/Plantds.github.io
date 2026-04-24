#pragma once

struct sPlatforms
{
	tg::cVector3f platform_pos = {};

	tg::cVector3f m_prev_pos = {};
	tg::cVector3f m_next_pos = {};

	float diff                      = 0.0f;
	float m_time                    = 0.0f;
	float m_point_travel_time       = 1.5f;

	int m_point_index       = 0; 
	int m_prev_point_index  = 0;
	int m_next_point_index  = 0;
	int m_max_size_of_index = 0; 

	bool m_is_active = false;
	bool m_has_reached_destination = false;
	bool m_lerp = true;

	enum state {
		isEnd,
		isStart,
		isMoving
	};

	std::shared_ptr<cTriggerNode> trigger = nullptr;

	state m_current_state = state::isStart;

	//for debug
	bool debug_mode = true;
};


class cPlatforms : public cModelNode, public tg::iInputListener
{
public:

	//////////////////////////////////////////////////////////////////////////
	
	cPlatforms(const int& _uuid, const int& _asset_ID, const std::string& _name, const std::weak_ptr<cScene> _scene);
	~cPlatforms();

	bool onInputEvent(const uint32_t _type, const tg::Input::sEvent& _event) override;

	void update(const float& _delta_time) override;

	void checkForPlayer(tg::cLine3f _player_line);

	void checkForOrb();

	void updatePlayerLine(tg::cLine3f& _player_line);
	void updateOrbsLine(std::vector<tg::cLine3f>& _orb_line);


	void addTrigger(const std::weak_ptr<cScene> _scene);
	void GetPoints(const std::weak_ptr<cScene> m_scene);
	void changeIndex(const int& _flow_of_time);
	void stateMachine(const sPlatforms::state& _state, const float& _delta_time);
	void lerpBetweenPoints();

	void bindPlayer(const std::shared_ptr<iNode>& _player) { player_ptr = _player; };
	void bindOrbHandler(const std::shared_ptr<cOrbHandler>& _orb_handler) { orb_handler_ptr = _orb_handler; };


	//void switchDirection(sPlatforms& _platform );

	////////////////////////////////////////////////////////////////////////// 

	sPlatforms platform = {};

	tg::cLine3f player_line = {};

	std::shared_ptr<cOrbHandler> orb_handler_ptr = nullptr;

	std::vector<tg::cLine3f> orb_lines = {};

	std::shared_ptr<iNode> player_ptr = nullptr;

	bool is_player_child = false;

	bool has_changed = false;
	int indexx = 0;
	//////////////////////////////////////////////////////////////////////////

private:
	std::vector<tg::cVector3f> positions = {};

};