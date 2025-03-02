#!/bin/bash
k3d cluster delete mycluster
k3d cluster create mycluster --agents 2 -p "80:80@loadbalancer"
