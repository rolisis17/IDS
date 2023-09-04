#!/bin/bash

LOG_FILE="/var/log/auth.log"

# Check if the log file exists and is readable
if [ -r "$LOG_FILE" ]; then
  # Log file exists and is readable

  # Monitor failed login attempts
  tail -n 0 -f "$LOG_FILE" | \
    while read line; do
      if echo "$line" | grep -q "authentication failure"; then
        echo "Suspicious login attempt detected: $line"
        # You can add more sophisticated alerting mechanisms here
      fi
    done
else
  exit 1;
#   echo "Error: Log file $LOG_FILE does not exist or is not readable."
#   # Add your error handling or logging here
fi
