#!/bin/bash

# Monitor failed login attempts
tail -n 0 -f /var/log/auth.log | \
  while read line; do
    if echo "$line" | grep -q "authentication failure"; then
      echo "Suspicious login attempt detected: $line"
      # You can add more sophisticated alerting mechanisms here
    fi
  done
