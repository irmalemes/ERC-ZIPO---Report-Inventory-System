# Report Inventory System

Internship project for building a desktop application that manages recurring PIO/MIO, CBBH and Halcom reports.

## Project goals
- Upload Word/PDF reports
- Automatically identify report type and date from filename
- Store report metadata in SQLite
- Organize files on disk
- Show file size, upload date and report date/day
- Track missing reports for each reporting period
- Remind users on the 12th and 22nd of each month
- Prepare/send a CBBH announcement email

## Report categories
1. **PIO/MIO**
   - `Izvještaj FZ MIOPIO SARAJEVO dd.mm.yyyy`
   - `Izvještaj FZ MIOPIO MOSTAR dd.mm.yyyy`
2. **CBBH**
   - `Zapisnik pregleda sistema BPRV dd.mm.yyyy`
3. **Halcom**
   - `report dd.mm.yyyy`
   - `CBBH Infrastructure Report yyyy-mm-dd`

## Suggested stack
- C++17+
- Qt 6
- SQLite
- CMake
- Git/GitHub

## Development rules
- Work on a milestone branch, not directly on `main`.
- Make small, meaningful commits.
- Open a Pull Request when a milestone is complete.
- Add/update tests for new logic.
- Never commit passwords, API keys, SMTP credentials, real reports or other confidential data.

## Repository structure
See `docs/development-workflow.md` and `docs/milestones.md`.
