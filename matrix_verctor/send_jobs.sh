#!/bin/bash

# Specify the folder containing your files
folder_path="/path/to/your/folder"

# Check if the folder exists
if [ ! -d "$folder_path" ]; then
    echo "Folder not found: $folder_path"
    exit 1
fi

# Loop through each file in the folder
for file in "$folder_path"/*; do
    if [ -f "$file" ]; then
        # Run sbatch with the current file
        sbatch "$file"
        echo "Submitted job for $file"
    fi
done
