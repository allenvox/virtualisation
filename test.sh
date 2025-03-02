#!/bin/bash
echo "Testing myapp.local (should return random number):"
curl http://myapp.local
echo ""
echo "Testing health (should return OK):"
curl http://myapp.local/health
echo ""
