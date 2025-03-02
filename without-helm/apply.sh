#!/bin/bash
for var in "$@"
do
    kubectl apply -f $var.yaml
done
