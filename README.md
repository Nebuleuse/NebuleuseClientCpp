# NebuleuseClient
Nebuleuse client written in Cpp
# Installation & building
This client has multiple dependencies : Rapidjson, cURL, Boost. cURL and rapidjson are included as submodules. Execute
git submodule init
git submodule update
To fetch the submodules. You will need to build cURL independently.
Boost is not included in this repo and you will need to download it yourself on the Boost website and build it yourself. Version used is 1.57.
