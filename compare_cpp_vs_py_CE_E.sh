#!/bin/bash

# Base paths
cpp_dir="./output/stage_1_tower_sums/CE_E"
python_dir="./python/output/Stage1_TowerSums"

# Loop over sectors (0 to 6)
for sector in {0..2}; do
    # Loop over boards (0 to 13)
    for board in {0..13}; do
        # Construct filenames
        file_cpp="${cpp_dir}/TowerSums_Sector_${sector}_Board_${board}_CE_E.txt"
        file_python="${python_dir}/TowerSums_Sector_${sector}_Board_${board}_CE_E.txt"
        
        # Check if both files exist
        if [[ -f "$file_cpp" && -f "$file_python" ]]; then
            # Compare files
            if diff -q "$file_cpp" "$file_python" > /dev/null; then
                echo "Files match: Sector ${sector}, Board ${board}"
            else
                echo "Files differ: Sector ${sector}, Board ${board}"
            fi
        else
            # Report missing files
            if [[ ! -f "$file_cpp" ]]; then
                echo "Missing file in current directory: $file_cpp"
            fi
            if [[ ! -f "$file_python" ]]; then
                echo "Missing file in AFS directory: $file_python"
            fi
        fi
    done
done
