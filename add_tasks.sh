#!/bin/bash

# Check if the number parameter is provided
if [ -z "$1" ]; then
  echo "Usage: $0 <day-number>"
  exit 1
fi

# Extract the number from the first argument
YEAR=2024 # Change default AOC year if needed
number=$1

# Check if jq is installed
if ! command -v jq &> /dev/null; then
  echo "Error: jq is not installed. Please install jq and try again."
  exit 1
fi

# Path to the tasks.json file
tasks_file=".vscode/tasks.json"

# Check if the tasks.json file exists
if [ ! -f "$tasks_file" ]; then
  echo "Error: $tasks_file not found!"
  exit 1
fi

# Define the new task as a JSON object
new_task=$(cat <<EOF
{
    "label": "advent of code ${YEAR}-day${number}",
    "type": "shell",
    "linux": {
        "command": [
        "meson setup \$ADVENT_OF_CODE_${YEAR}/build \$ADVENT_OF_CODE_${YEAR} &&",
        "ninja -C \$ADVENT_OF_CODE_${YEAR}/build &&",
        "./\$ADVENT_OF_CODE_${YEAR}/build/day${number}"
        ]
    },
    "dependsOn": [],
    "problemMatcher": []
}
EOF
)

# Use jq to append the new task to the tasks array in tasks.json
jq --indent 4 --argjson new_task "$new_task" '.tasks += [$new_task]' "$tasks_file" > "$tasks_file.tmp" && mv "$tasks_file.tmp" "$tasks_file"

# Path to the meson.build file
meson_file="advent-of-code-${YEAR}/meson.build"

# Define the new program entry for meson.build
new_program="['day${number}', ['day${number}.cc']]"

# Determine the indentation level from line 5 onwards
indent=$(sed -n '5s/^\([\t ]*\).*/\1/p' "$meson_file")

# Append the new program entry to the progs array in meson.build
sed -i "/progs = \[/a \\
${indent}${new_program}," "$meson_file"

echo "Task for day${number} added successfully."
