#!/usr/bin/env python3
"""
Script to create complete project structure for S32K automotive project
(Scripts, CI, Tools, Simulation, Docs)
Run this script from your git repository root directory
"""

import os
from pathlib import Path

def create_project_structure():
    """Create the complete project folder structure"""
    
    # Define all files with their paths
    files = [
        # Scripts
        "scripts/build.sh",
        "scripts/build_S32K344.sh",
        "scripts/build_S32K348.sh",
        "scripts/run_tests.sh",
        "scripts/static_analysis.sh",
        "scripts/mcdc_coverage.sh",
        "scripts/platform_switch.sh",
        "scripts/calibration_flash.sh",
        "scripts/report_gen.py",
        "scripts/hse_key_init.sh",
        "scripts/lockstep_test.sh",
        "scripts/ethernet_tsn_config.sh",
        "scripts/flash_via_ethernet.sh",
        "scripts/qspi_flash_program.sh",
        "scripts/memory_layout_select.sh",
        "scripts/generate_safety_report.py",
        "scripts/trace_requirements.py",
        
        # CI/CD Workflows
        "ci/.github/workflows/build.yml",
        "ci/.github/workflows/build_S32K344.yml",
        "ci/.github/workflows/build_S32K348.yml",
        "ci/.github/workflows/test.yml",
        "ci/.github/workflows/static_analysis.yml",
        "ci/.github/workflows/coverage.yml",
        "ci/.github/workflows/platform_matrix.yml",
        "ci/.github/workflows/calibration_flash.yml",
        "ci/.github/workflows/hse_diag.yml",
        "ci/.github/workflows/lockstep_validation.yml",
        "ci/.github/workflows/ethernet_tsn_test.yml",
        "ci/.github/workflows/fault_injection.yml",
        "ci/.github/workflows/safety_verification.yml",
        "ci/.github/workflows/misra_compliance.yml",
        "ci/.github/workflows/requirements_trace.yml",
        
        # Tools - Cppcheck
        "tools/cppcheck/config.xml",
        "tools/cppcheck/results.txt",
        "tools/cppcheck/misra_rules.xml",
        
        # Tools - LDRA
        "tools/ldra/config.xml",
        "tools/ldra/results.txt",
        "tools/ldra/mcdc_report.html",
        
        # Tools - Polyspace
        "tools/polyspace/config.xml",
        "tools/polyspace/results.txt",
        
        # Tools - Calibration
        "tools/calibration/uds_xcp_adapter.py",
        "tools/calibration/calibration_report_gen.py",
        "tools/calibration/calibration_verify.py",
        "tools/calibration/ethernet_xcp_adapter.py",
        
        # Tools - HSE
        "tools/hse/hse_keytool.py",
        "tools/hse/hse_diag_report_gen.py",
        
        # Tools - Lockstep
        "tools/lockstep/lockstep_analyzer.py",
        "tools/lockstep/lockstep_fault_injector.py",
        "tools/lockstep/lockstep_report_gen.py",
        
        # Tools - Ethernet
        "tools/ethernet/tsn_configurator.py",
        "tools/ethernet/ptp_sync_analyzer.py",
        "tools/ethernet/ethernet_traffic_gen.py",
        "tools/ethernet/wireshark_filters/tsn_filter.lua",
        "tools/ethernet/wireshark_filters/e2e_filter.lua",
        
        # Tools - Trace
        "tools/trace/requirements_tracer.py",
        "tools/trace/coverage_tracer.py",
        "tools/trace/safety_tracer.py",
        
        # Tools - Certificates
        "tools/certificates/ldra_qualification.pdf",
        "tools/certificates/cppcheck_certificate.pdf",
        "tools/certificates/polyspace_certificate.pdf",
        "tools/certificates/calibration_data_verified.pdf",
        "tools/certificates/safety_certification_tuv.pdf",
        "tools/certificates/iso26262_compliance.pdf",
        
        # Simulation - Models
        "simulation/models/vehicle_dynamics.slx",
        "simulation/models/powertrain_model.slx",
        "simulation/models/brake_system.slx",
        
        # Simulation - SIL
        "simulation/sil/sil_wrapper.c",
        "simulation/sil/sil_config.yaml",
        
        # Simulation - HIL
        "simulation/hil/hil_test_bench.yaml",
        "simulation/hil/hil_scenarios/emergency_brake.yaml",
        "simulation/hil/hil_scenarios/power_loss.yaml",
        "simulation/hil/hil_scenarios/lockstep_fault.yaml",
        
        # Documentation
        "docs/user_manual.md",
        "docs/operator_guide.md",
        "docs/troubleshooting.md",
        "docs/diagnostic_workflows.md",
        "docs/calibration_workflows.md",
        "docs/hse_manual.md",
        "docs/lockstep_manual.md",
        "docs/ethernet_tsn_manual.md",
        "docs/safety_manual.md",
        
        # Documentation - API Reference
        "docs/api_reference/mcal_api.md",
        "docs/api_reference/safetylib_api.md",
        "docs/api_reference/lockstep_api.md",
        "docs/api_reference/ethernet_api.md",
        
        # Documentation - Certification
        "docs/certification/asil_d_evidence.md",
        "docs/certification/misra_compliance.md",
        "docs/certification/tool_qualification.md",
        "docs/certification/functional_safety_assessment.pdf",
    ]
    
    print("Creating project structure (scripts, CI, tools, simulation, docs)...")
    print("=" * 70)
    
    # Create all files
    file_count = 0
    skip_count = 0
    
    for file_path in files:
        try:
            file = Path(file_path)
            
            # Create parent directories if they don't exist
            file.parent.mkdir(parents=True, exist_ok=True)
            
            # Create empty file if it doesn't exist
            if not file.exists():
                # Write empty content to file
                with open(file, 'w') as f:
                    f.write('')
                print(f"✓ Created: {file_path}")
                file_count += 1
            else:
                print(f"- Skipped (exists): {file_path}")
                skip_count += 1
                
        except Exception as e:
            print(f"✗ ERROR creating {file_path}: {e}")
    
    print("\n" + "=" * 70)
    print(f"✅ Project structure created successfully!")
    print(f"   Created: {file_count} new files")
    print(f"   Skipped: {skip_count} existing files")
    print("=" * 70)
    
    print("\n📁 Directory Summary:")
    print(f"   - scripts/           : {len([f for f in files if f.startswith('scripts/')])} files")
    print(f"   - ci/                : {len([f for f in files if f.startswith('ci/')])} files")
    print(f"   - tools/             : {len([f for f in files if f.startswith('tools/')])} files")
    print(f"   - simulation/        : {len([f for f in files if f.startswith('simulation/')])} files")
    print(f"   - docs/              : {len([f for f in files if f.startswith('docs/')])} files")
    
    print("\n⚠️  Note: Make shell scripts executable:")
    print("   chmod +x scripts/*.sh")
    
    print("\n📝 Next steps:")
    print("   1. Verify files: ls -la scripts/ ci/ tools/ simulation/ docs/")
    print("   2. Make scripts executable: find scripts -name '*.sh' -exec chmod +x {} \\;")
    print("   3. Stage files: git add scripts/ ci/ tools/ simulation/ docs/")
    print("   4. Commit: git commit -m 'Add project structure'")
    print("   5. Push: git push origin main")

if __name__ == "__main__":
    try:
        create_project_structure()
    except Exception as e:
        print(f"❌ Fatal Error: {e}")
        import traceback
        traceback.print_exc()
        exit(1)