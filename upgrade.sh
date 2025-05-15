#!/bin/bash
helm upgrade my-app ./my-app-chart -f my-app-chart/values.yaml
helm upgrade my-app ./my-app-chart -f my-app-chart/custom-values.yaml
