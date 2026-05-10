import subprocess
import os
from pathlib import Path

Import("env")

def get_firmware_specifier_build_flag():
    build_version = "v1.0.0-dev" # Default fallback

    try:
        # Try git describe first
        ret = subprocess.run(["git", "describe", "--tags", "--always", "--dirty"],
                           stdout=subprocess.PIPE,
                           stderr=subprocess.DEVNULL,
                           text=True)
        if ret.returncode == 0 and ret.stdout.strip():
            build_version = ret.stdout.strip()
    except Exception:
        pass

    # If git failed or returned "dev", try to get it from version.h
    if build_version == "v1.0.0-dev" or build_version == "dev":
        try:
            version_h = Path(env.subst("$PROJECT_DIR")) / "src" / "version.h"
            if version_h.exists():
                with open(version_h, 'r') as f:
                    for line in f:
                        if "CURRENT_VERSION" in line:
                            import re
                            match = re.search(r'"([^"]+)"', line)
                            if match:
                                build_version = match.group(1)
                                break
        except Exception:
            pass

    # Clean up version string
    build_version = build_version.replace('Release', '').replace('release', '')

    build_flag = "-D AUTO_VERSION=\\\"" + build_version + "\\\""
    print ("Firmware Revision: " + build_version)
    return (build_flag)

env.Append(
    BUILD_FLAGS=[get_firmware_specifier_build_flag()]
)
