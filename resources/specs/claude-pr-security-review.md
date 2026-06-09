Review the complete PR diff. This contains all code changes in the PR.

# OBJECTIVE

Perform a security-focused code review to identify HIGH-CONFIDENCE security vulnerabilities that could have real exploitation potential.

This is not a general code review - focus ONLY on security implications newly added by this PR. Do not comment on existing security concerns.

For this task, you will rely on two additional files, `resources/specs/claude-security-scan-instructions.md` detailing security scan instructions specific to the repository and `resources/specs/claude-false-positive-filtering.md` detailing patterns, precedents, exclusion rules based on which some of the identified vulnerabilities will be excluded from the final reporting. The instructions, rules and criterias found in those two files are MANDATORY TO BE HONOURED and your security audit will have to take all of them into account.

YOU MUST READ AND ANALYZE BOTH OF THESE FILES BEFORE COMMENCING THE SECURITY AUDIT. THESE FILES, ALONG WITH SECTIONS FROM THEM, WILL BE MENTIONED IN THE FOLLOWING PARTS OF THIS PROMPT, PROVIDING FULL CONTEXT FOR ACCOMPLISHING THE GIVEN TASK.

# CRITICAL INSTRUCTIONS
1. MINIMIZE FALSE POSITIVES: Only flag issues where you're >80% confident of actual exploitability
2. AVOID NOISE: Skip theoretical issues, style concerns, or low-impact findings
3. UNDERSTANDING CONTEXT AND SCOPE OF REVIEW: In the security scan instructions file `resources/specs/claude-security-scan-instructions.md`, read the `External Attacker Threat Model` and `Mongoose-Specific Security Review Scope` subsections found in the `SECURITY SCAN CONTEXT` main section for a full understanding of the scanning model, code review context and the perspective from which the scanning is going to take place.
4. TREAT EXCLUSIONS AS MANDATORY. The exclusions section below is mandatory. If a potential finding falls under an exclusion, do not report it. If a finding partially overlaps an exclusion, only report it if there is a concrete, non-excluded security impact.

# SECURITY CATEGORIES TO EXAMINE 

In the security scan instructions file `resources/specs/claude-security-scan-instructions.md`, read all the security categories found in the `SECURITY CATEGORIES TO EXAMINE` main section. THESE ARE THE SECURITY CATEGORIES WHICH NEED TO BE EXAMINED.

Use these categories to guide prioritization, not as a checklist requiring equal coverage of every item.

# ANALYSIS METHODOLOGY

Phase 1 - Repository Context Research:
- Identify existing security frameworks and libraries in use
- Look for established secure coding patterns in the codebase
- Examine existing sanitization and validation patterns
- Understand the project's security model and threat model

Phase 2 - Comparative Analysis:
- Compare new code changes against existing security patterns
- Identify deviations from established secure practices
- Look for inconsistent security implementations
- Flag code that introduces new attack surfaces

Phase 3 - Vulnerability Assessment:
- Examine each modified file for security implications
- Trace data flow from user inputs to sensitive operations
- Look for privilege boundaries being crossed unsafely
- Identify injection points and unsafe deserialization

# REQUIRED OUTPUT FORMAT

You MUST post your findings as a single top-level PR comment using:

`gh pr comment ${{ env.PR_NUMBER }} --repo ${{ github.repository }} --body-file <file>`

The PR comment body MUST be readable GitHub-flavored Markdown.

Start with this exact heading:

`## Claude PR Security Review`

Then include this summary section first:

`### Analysis Summary`

- Files reviewed: <number>
- High severity findings: <number>
- Medium severity findings: <number>
- Low severity findings: <number>
- Review completed: true|false

Then include the findings section:

`### Findings`

If there are no findings, write exactly:

No security findings identified.

If there are findings, list each finding using this format:

`#### <severity>: <brief vulnerability title>`

- **Confidence:** <0.0-1.0>
- **File:** `<path/to/file>`
- **Line:** <line number>
- **Description:** <clear explanation of the vulnerability>
- **Attack scenario:** <how an attacker could trigger or exploit this issue>
- **Impact:** <concrete security impact>
- **Recommendation:** <specific remediation guidance>
- **Evidence:** <relevant code behavior or data flow supporting the finding>

Do not include raw JSON in the PR comment.
Do not include any text before the `## Claude PR Security Review` heading.
Do not include findings below 0.7 confidence.

# SEVERITY GUIDELINES
- **HIGH**: Directly exploitable vulnerabilities leading to RCE, data breach, or authentication bypass
- **MEDIUM**: Vulnerabilities requiring specific conditions but with significant impact
- **LOW**: Defense-in-depth issues or lower-impact vulnerabilities

# CONFIDENCE SCORING
- 0.9-1.0: Certain exploit path identified, tested if possible
- 0.8-0.9: Clear vulnerability pattern with known exploitation methods
- 0.7-0.8: Suspicious pattern requiring specific conditions to exploit
- Below 0.7: Don't report (too speculative)

When judging confidence, also take into account the signal quality criteria found in the `SIGNAL QUALITY CRITERIA` section of the  `resources/specs/claude-false-positive-filtering.md` filtering file.

# FINAL REMINDER: Focus on HIGH and MEDIUM findings only.

Better to miss some theoretical issues than flood the report with false positives. Each finding should be something a security engineer would confidently raise in a PR review.

# IMPORTANT EXCLUSIONS - DO NOT REPORT:

Read the `HARD EXCLUSIONS` section found in the `resources/specs/claude-false-positive-filtering.md` filtering file, for each possbile vulnerability found, check if it matches any items from the patterns listed in that section and if that is the case, exclude that vulnerability from the report. DO NOT REPORT IT if a match is found. Also, take into account the project-specific filtering and reporting precedents found in the `PRECEDENTS` section of the filtering file.

Begin your analysis now. Use the repository exploration tools to understand the codebase context, then analyze the PR changes for security implications.

Your final action must be posting the PR comment. You should not reply again after posting the PR comment.