<!-- omit in toc -->
# Contributing to SplitFlap

First off, thanks for taking the time to contribute!

All types of contributions are encouraged and valued. See the [Table of Contents](#table-of-contents) for different ways to help and details about how this project handles them. Please make sure to read the relevant section before making your contribution. It will make it a lot easier for the maintainer and smooth out the experience for everyone involved.

> If you like the project but don't have time to contribute, that's fine too. Starring the repo, sharing it, or mentioning it to others who might be into split-flap displays or model railway projects is appreciated just as much.

<!-- omit in toc -->
## Table of Contents

- [I Have a Question](#i-have-a-question)
- [I Want To Contribute](#i-want-to-contribute)
  - [Reporting Bugs](#reporting-bugs)
  - [Suggesting Enhancements](#suggesting-enhancements)
  - [Creating a Pull Request](#creating-a-pull-request)
    - [Before Creating a Pull Request](#before-creating-a-pull-request)
    - [How Do I Submit a Good Pull Request?](#how-do-i-submit-a-good-pull-request)

## I Have a Question

> If you want to ask a question, please check the [README](https://github.com/SahibManjal/SplitFlap) first, it covers the hardware setup, wiring, and firmware flashing process.

Before opening a new issue, search existing [Issues](https://github.com/SahibManjal/SplitFlap/issues) to see if your question has already been answered.

If you still need help:

- Open an [Issue](https://github.com/SahibManjal/SplitFlap/issues/new).
- Provide as much context as you can: what board you're using (e.g. ESP32/Arduino variant), what firmware version, and what you were trying to do.
- Include any relevant logs, serial monitor output, or error messages.

## I Want To Contribute

> ### Legal Notice
> When contributing to this project, you must agree that you have authored 100% of the content, that you have the necessary rights to the content, and that it may be provided under the project's license.

### Reporting Bugs

<!-- omit in toc -->
#### Before Submitting a Bug Report

A good bug report saves everyone time. Before filing one, please:

- Make sure you're running the latest firmware/backend code from `main`.
- Check whether it's a hardware/wiring or other external issue rather than a firmware bug (loose connections, incorrect board settings, inconsistent WiFi, power supply issues are common culprits).
- Search existing [Issues](https://github.com/SahibManjal/SplitFlap/issues) to see if it's already been reported.
- Collect relevant details:
  - Board/microcontroller (e.g. ESP32 variant, Arduino model)
  - Firmware version / commit hash
  - Serial monitor output or stack trace, if available
  - Python version and OS, if the issue is in the backend/scraper
  - Steps to reliably reproduce the issue

<!-- omit in toc -->
#### How Do I Submit a Good Bug Report?

> Please do not report security-related issues in the public issue tracker. Instead, contact ssm237@cornell.edu directly.

- Open an [Issue](https://github.com/SahibManjal/SplitFlap/issues/new).
- Describe the expected behavior vs. the actual behavior.
- Include reproduction steps and the details collected above.
- If it's a hardware behavior issue (e.g. flap misalignment, motor stalling), a photo or short video is extremely helpful.

### Suggesting Enhancements

This section covers suggestions for new features or improvements: whether to the firmware, the backend, or the physical hardware design.

<!-- omit in toc -->
#### Before Submitting an Enhancement

- Check the [README](https://github.com/SahibManjal/SplitFlap) and existing [Issues](https://github.com/SahibManjal/SplitFlap/issues) to see if it's already covered or suggested.
- Consider whether the enhancement is broadly useful (e.g. supporting another board, improving reliability of the position tracking logic) versus a one off customization better suited to your own fork.

<!-- omit in toc -->
#### How Do I Submit a Good Enhancement Suggestion?

- Open an [Issue](https://github.com/SahibManjal/SplitFlap/issues/new) with a clear, descriptive title.
- Describe the enhancement step by step and why it would be useful.
- If it touches hardware (mechanism, wiring, PCB), diagrams or photos help a lot.
- If it touches firmware or backend logic, pseudocode or a rough implementation sketch is welcome.

### Creating a Pull Request

#### Before Creating a Pull Request

- Check for an existing [Issue](https://github.com/SahibManjal/SplitFlap/issues) or [Pull Request](https://github.com/SahibManjal/SplitFlap/pulls) covering the same change.
- For larger changes (new hardware support, backend architecture changes), consider opening an issue first to discuss the approach.

#### How Do I Submit a Good Pull Request?

- Use a clear, descriptive title.
- Link the related Issue, if one exists.
- Describe what the change does and why.
- Keep firmware changes and backend/Python changes in separate PRs where possible, since they're reviewed differently.
- Comment non-obvious code, especially timing sensitive or hardware specific logic (e.g. motor stepping, tick based position updates, ...).
- Test on real hardware where feasible and note what you tested in the PR description.

<!-- omit in toc -->
## Attribution
This guide is adapted from the [contributing-gen](https://github.com/bttger/contributing-gen) template.