# Security Policy

## Supported Versions

| Version | Supported |
| ------- | --------- |
| 2.x     | Yes       |
| 1.x     | Yes       |

Unlike a typical versioning scheme, releases here aren't strictly sequential upgrades, as each represents a
distinct hardware/software configuration. v1.x is fully self contained on the ESP32, with no external backend.
v2.x adds a Raspberry Pi backend for holiday-aware scheduling, delay tracking, and other features that need
more than the ESP32 can do on its own.

Because of this, running v1.x isn't "outdated". It's a valid, simpler setup for anyone who doesn't want the
Pi based components. Both lines receive security fixes independently. This table will be updated if that
changes as the project evolves.

## Scope

In scope: the firmware (ESP32/Arduino code) and the Python backend in this repository.

Since this project controls physical hardware (motors, flap mechanisms) and talks to external APIs
(train data sources), things I'd consider a vulnerability include:

- Anything that could cause unsafe physical behavior (e.g. motor never stopping, runaway current draw)
- Hardcoded credentials, tokens, or API keys committed to the repo
- Unvalidated input from external data sources (train APIs, scraped pages) that could crash the device or
  corrupt its state
- Insecure handling of WiFi credentials or network communication on the ESP32 side

Out of scope: issues that only affect a modified/forked version of this hardware or firmware, and general
bugs with no security impact (those should just go in the normal issue tracker).

## Reporting a Vulnerability

If you find a security issue, please **don't open a public GitHub issue** for it.

Instead:

- **Preferred:** [report it privately on GitHub](https://github.com/SahibManjal/SplitFlap/security/advisories/new).
  This opens a private thread just between us.
- **By email:** ssm237@cornell.edu, with `SECURITY` somewhere in the subject line.

When you report, it helps to include:

- What commit/version you're running
- A description of the issue and what you think the impact is
- Steps to reproduce, if applicable
- Any relevant logs or serial output

## What to Expect

This is a three person hobby project maintained in spare time, so please bear with realistic expectations:

- **Acknowledgement:** I'll aim to reply within a week.
- **Fix timeline:** no fixed SLA, it will depend on severity and my available time. If it's something that could
  cause unsafe physical behavior, I'll prioritize it.
- **Disclosure:** I'd appreciate advance notice before anything is published publicly, but there's no formal
  embargo process here. If I go quiet for an extended period, feel free to publish responsibly.
- **Credit:** happy to credit you in the fix commit/changelog unless you'd rather stay anonymous.

There's no bug bounty, this is an unfunded personal project.

## Contact

- **Security reports:** ssm237@cornell.edu, or
  [privately on GitHub](https://github.com/SahibManjal/SplitFlap/security/advisories/new)