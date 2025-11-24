# Contributors Guide

## Development Standards

### Coding Standards
- **Language**: C11 (ISO/IEC 9899:2011)
- **Style Guide**: MISRA C:2012 (mandatory rules + selected advisory)
- **Naming**: snake_case for functions/variables, PascalCase for types
- **Documentation**: Doxygen format for all public APIs

### Safety Requirements
- All ASIL-D modules require >95% MC/DC coverage
- Static analysis must pass without deviations
- Code reviews required for all safety-critical code
- Traceability to requirements mandatory

### Git Workflow
1. Create feature branch from `develop`
2. Implement with unit tests
3. Run static analysis and tests locally
4. Create pull request
5. Await peer review
6. Merge to `develop`

## Contact

- GitHub Issues: Primary communication
