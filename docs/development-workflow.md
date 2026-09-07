# Development Workflow

## Branch naming
Use:
`milestone/01-requirements`
`milestone/02-cpp-model`
...
`milestone/10-email`

## Commit examples
- `Initial project structure`
- `Add Report model`
- `Implement filename parser`
- `Add SQLite repository`
- `Implement upload workflow`

## Pull Requests
A milestone PR should contain:
- What was implemented
- How it was tested
- Known limitations
- Screenshots when GUI changes are involved

## Security
Never commit:
- passwords
- API keys
- SMTP credentials
- real client reports
- confidential customer information

Use `config/config.example.json` as the template for local configuration.
