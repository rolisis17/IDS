#!/bin/bash

# Monitor failed login attempts
journalctl | \
  while read line; do
    if echo "$line" | grep -q "authentication failure"; then
      echo "Suspicious login attempt detected: $line"
      # You can add more sophisticated alerting mechanisms here
    fi
  done
