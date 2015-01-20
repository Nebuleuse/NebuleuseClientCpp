#include "Nebuleuse.h"
#include "rapidjson/document.h"

namespace Neb{
	using namespace rapidjson;
	void Nebuleuse::Parse_Status(std::string data){
		Document doc;
		if (doc.Parse(data.c_str()).HasParseError()){
			return ThrowError(NEBULEUSE_ERROR_PARSEFAILED);
		}
		if (!doc.IsObject()){
			return ThrowError(NEBULEUSE_ERROR_PARSEFAILED);
		}
		if (doc.HasMember("Maintenance") && doc["Maintenance"].IsBool()){
			if (doc["Maintenance"].GetBool())
				return ThrowError(NEBULEUSE_ERROR_MAINTENANCE);
		}
		if (doc.HasMember("NebuleuseVersion") && doc["NebuleuseVersion"].IsInt()){
			if (doc["NebuleuseVersion"].GetInt() > NEBULEUSEVERSION)
				return ThrowError(NEBULEUSE_ERROR_OUTDATED);
		}
		if (doc.HasMember("GameVersion") && doc["GameVersion"].IsUint()){
			if (doc["GameVersion"].GetUint() > m_uiVersion)
				return ThrowError(NEBULEUSE_ERROR_OUTDATED);
		}
		if (doc.HasMember("Motd") && doc["Motd"].IsString()){
			m_cMotd = doc["Motd"].GetString();
		}
			
		return;
	}
	void Nebuleuse::Parse_Connect(std::string data){
		Document doc;
		if (doc.Parse(data.c_str()).HasParseError()){
			return ThrowError(NEBULEUSE_ERROR_PARSEFAILED);
		}
		if (!doc.IsObject()){
			return ThrowError(NEBULEUSE_ERROR_PARSEFAILED);
		}
		if (doc.HasMember("Code") && doc.HasMember("Message")){
			if (doc["Code"].IsInt()){
				if (doc["Code"].GetInt() == NEBULEUSE_ERROR_LOGIN)
					return ThrowError(NEBULEUSE_ERROR_LOGIN);			
				return ThrowError(NEBULEUSE_ERROR);
			}
		}
		if (!doc.HasMember("SessionId") || !doc["SessionId"].IsString()){
			return ThrowError(NEBULEUSE_ERROR_PARSEFAILED);
		}
		m_cSessionID = doc["SessionId"].GetString();

		return;
	}
}