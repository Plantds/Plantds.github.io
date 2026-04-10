(function() {
    var e = function() {
        var r = document.getElementById("ascii_canvas")
        , o = void 0
        , c = void 0
        , e = 1
        , t = 0
        , n = 0
        , s = 1
        , i = function(e, t, n) {
            var s = t;
            return t -= e * n,
            n += e * s,
            s = (3 - t * t - n * n) / 2,
            t *= s,
            n *= s,
            [t, n]
        }
        , a = function() {
            m = [],
            h = [],
            o = i(.04, e, t),
            e = o[0],
            t = o[1],
            o = i(.02, n, s),
            n = o[0],
            s = o[1];
            for (u = 0; u < 1760; u++)
                 m[u] = u % 80 == 79 ? `
` : " ", // makes multiple dounts
            h[u] = 0;
            for (var o, a, c, u, h, m, v, l = 0, d = 1, y = 0; y < 90; y++) { // 90 full depth
            for (a = 0, c = 1, v = 0; v < 314; v++) { // 314 full circle
                    var f = d + 2
                    , p = 1 / (a * f * t + l * e + 5)
                    , _ = a * f * e - l * t
                    , j = 0 | 40 + 30 * p * (c * f * n - _ * s) // horizontal
                    , g = 0 | 12 + 15 * p * (c * f * s + _ * n) // vertical
                    , b = j + 80 * g //character pos
                    , w = 0 | 8 * ((l * t - a * d * e) * n - a * d * t - l * e - c * d * s); // Character selection
                    g < 22 && g >= 0 && j >= 0 && j < 79 && p > h[b] && (h[b] = p, m[b] = ".,-~:;=!*#$@"[w > 0 ? w : 0]),
                    o = i(.02, c, a),
                    c = o[0],
                    a = o[1]
                }
                o = i(.07, d, l),
                d = o[0],
                l = o[1]
            }
                r.innerHTML = m.join("")
            };
                window.anim1 = function() {
                    o == void 0 ? o = setInterval(a, 50) : (clearInterval(o), o = void 0)
                }
        a(),
        window.anim1()
     };
                document.all ? window.attachEvent("onload", e) : window.addEventListener("load", e, !1)
    }
)()