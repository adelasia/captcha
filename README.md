# captcha

DayZ captcha mod, blocks bots and NVIDIA Inspector LOD-bias abuse.

**License:** [CC BY-NC 4.0](LICENSE.txt)

![Captcha join screen](screenshots/screenshot1.png)

---

**Complementary mod:** [obscura](https://github.com/adelasia/obscura) - fullscreen overlay that blocks bots and LOD-bias abuse ingame.

---

### False positives

**At default settings, the digits render normally on any common resolution.**

Unlike [obscura](https://github.com/adelasia/obscura)'s fullscreen overlay, captcha has no
hard invisible/white cutoff - the digit glyphs are a normal texture that just blurs
progressively as LOD bias increases, then genuinely fades to fully transparent once bias
pushes far enough.

At default (zero) LOD bias, the natural mip level across common resolutions sits
comfortably below the fade threshold - no false positives. Triggering the fade
requires roughly **+1.6** LOD bias at 1280x720 or **+2.6** at 2560x1440 (scales with
resolution, same as the mod's own on-screen digit size).

**Either way, seeing the digits blur or fade means abnormal texture LOD is active.**

---

## How it works

- On connect, the client shows a 2-digit captcha before joining.
- Three attempts per session, correct answer required to continue.
- Captcha is shown once per game session until the client is restarted.

`LOD sensitivity varies by resolution; lower res needs less LOD bias, higher res needs more.`

<table>
  <tr>
    <td width="50%"><img src="screenshots/screenshot1.png" alt="Normal LOD bias" width="100%"/></td>
    <td width="50%"><img src="screenshots/screenshot2.png" alt="Higher LOD bias" width="100%"/></td>
  </tr>
  <tr>
    <td align="center"><b>Normal LOD bias (±0)</b></td>
    <td align="center"><b>Higher LOD bias (around +1.5 or higher)</b></td>
  </tr>
</table>

---

## Why do I need this?

This mod prevents players from using NVIDIA Inspector LOD bias to gain an unfair advantage.

<table>
  <tr>
    <td width="33%"><img src="screenshots/screenshot3.png" alt="Normal LOD bias" width="100%"/></td>
    <td width="33%"><img src="screenshots/screenshot4.png" alt="LOD bias +3" width="100%"/></td>
    <td width="33%"><img src="screenshots/screenshot5.png" alt="LOD bias +12" width="100%"/></td>
  </tr>
  <tr>
    <td align="center"><b>Normal LOD bias (±0)</b></td>
    <td align="center"><b>High LOD bias (+3)</b></td>
    <td align="center"><b>Higher LOD bias (+12)</b></td>
  </tr>
</table>

---

## Install

This repo is source only - grab the built mod from Steam Workshop, or build it
yourself from this source using DayZ Tools (AddonBuilder).

### Server

1. Add `@captcha` to your server mod list (Workshop) or your own built `@captcha` folder.
2. Place `captcha.bikey` into the server `keys/` folder.
3. Set `verifySignatures = 2` in `serverDZ.cfg`.

### Client

1. Subscribe on Steam Workshop **or** build `@captcha` yourself from this source.
2. Enable `@captcha` in the launcher / DZSA mod list.

A built client folder must include:

```
@captcha/
  addons/captcha.pbo
  addons/captcha.pbo.captcha.bisign
  keys/captcha.bikey
  meta.cpp
```

> Note: the signing key was rotated on 2026-07-14 - if you have an older
> `adelasia.bikey` from a previous release, replace it with `captcha.bikey`.

## Feedback

Report bugs by [opening an issue](https://github.com/adelasia/captcha/issues).

For suggestions, questions, or general chat, [start a discussion](https://github.com/adelasia/captcha/discussions).

---

## License

Forks and redistributions must credit adelasia and comply with [CC BY-NC 4.0](LICENSE.txt).
