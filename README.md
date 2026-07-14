# captcha

DayZ captcha mod, blocks bots and NVIDIA Inspector LOD-bias abuse.

**License:** [CC BY-NC 4.0](LICENSE.txt)

![Captcha join screen](screenshots/screenshot1.png)

---

**Complementary mod:** [obscura](https://github.com/adelasia/obscura) - fullscreen overlay that blocks bots and LOD-bias abuse ingame.

---

### False positives

**At default settings, there should not be any false positives on any resolution.**

A white screen only appears if Texture LOD Bias is raised.

The mod's visible mip is set to **3** (mips 0–2 are invisible, 3+ are white).

`x = log₂( max(2048 / width, 2048 / height) )`

At default settings, **x stays below 3 on all common resolutions**, no false positives.

**Either way, triggering means abnormal texture LOD is active.**

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

### Server

1. Copy the `@captcha` folder into your DayZ server directory.
2. Place `adelasia.bikey` from `@captcha/keys/` into the server `keys/` folder.
3. Add `@captcha` to your server mod list.
4. Set `verifySignatures = 2` in `serverDZ.cfg`.

### Client

1. Subscribe on Steam Workshop **or** copy `@captcha` into your DayZ mods folder.
2. Enable `@captcha` in the launcher / DZSA mod list.

The client folder must include:

```
@captcha/
  addons/captcha.pbo
  addons/captcha.pbo.adelasia.bisign
  keys/adelasia.bikey
  meta.cpp
```
## Feedback

Report bugs by [opening an issue](https://github.com/adelasia/dayz-captcha/issues).

For suggestions, questions, or general chat, [start a discussion](https://github.com/adelasia/dayz-captcha/discussions).

---

## License

Forks and redistributions must credit adelasia and comply with [CC BY-NC 4.0](LICENSE.txt).
