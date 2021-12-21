#include "include/vkapilib.h"

int main()
{

    vkapilib::VKAPI api("5.131");
	std::string params[][2] = {
		{"count", "5"},
		{"access_token", "token"},
		{"group_id", "194720349"}
	};

    std::string response = api.call("groups.getMembers", params, sizeof(params)/sizeof(params[0]));
	
	std::cout << response << std::endl;

}
