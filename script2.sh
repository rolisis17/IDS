#!/bin/bash

# Initialize a variable to track if any suspicious attempts were detected
suspicious_detected=0

# Monitor failed login attempts
journalctl -q | \
  while read line; do
    if echo "$line" | grep -q "authentication failure"; then
      echo "Suspicious login attempt detected: $line"
      suspicious_detected=1
      # You can add more sophisticated alerting mechanisms here
    fi
  done

# Check if any suspicious attempts were detected
if [ $suspicious_detected -eq 0 ]; then
  echo "No suspicious login attempts detected."
  # Add your action for no suspicious attempts here
fi
