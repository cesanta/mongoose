# Security Policy

Mongoose is a full-stack embedded networking library by Cesanta Software Ltd,
<https://mongoose.ws>. This policy covers how to report vulnerabilities
and how Cesanta handles them.

## Supported versions

Security fixes go to the `master` branch and the latest release.

## Reporting a vulnerability

Do not report security issues in public GitHub issues. Report them
privately using GitHub Security Advisories:
<https://github.com/cesanta/mongoose/security/advisories/new>

Include the Mongoose version or commit, steps or a proof of concept to reproduce, and
the impact. Cesanta usually responds the same business day.

## Disclosure process

1. Cesanta reviews the report and coordinates a fix with the reporter.
2. Eligible commercial customers receive a notification and a private fix
   one month before the CVE becomes public.
3. The fix and the CVE are then published. The reporter is credited in the
   GitHub Security Advisory.

## Security testing

- CI on every push, unit tests built with ASAN and UBSan
- CodeQL static analysis on every commit
- Continuous fuzzing on OSS-Fuzz, plus nightly libFuzzer runs
- AI-assisted security review on every pull request
- Regular review by independent security research groups worldwide

## EU Cyber Resilience Act (CRA)

Mongoose supports CRA compliance for connected products: a documented
vulnerability handling process (this file), TLS 1.3, signed OTA firmware
updates, and an SBOM (`sbom.cdx.json`). Details:
<https://mongoose.ws/security/cra-compliance/>
