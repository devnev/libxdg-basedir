#!/bin/sh

doxygen xdg.dox > /dev/null && echo "Documentation generated."
echo "---------------------------------------"
doxygen xdg-source.dox > /dev/null && echo "Source documentation generated."

