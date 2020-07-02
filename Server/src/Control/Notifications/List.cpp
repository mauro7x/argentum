#include "../../../includes/Control/Notifications/List.h"

List::List(const std::string& initial_msg,
           const std::list<std::string>& list_items) {
    j = json{{"init_msg", initial_msg}};
    j["list_items"] = json(list_items);
}

List::~List() {}

List::List(const List& other) {
    this->j = other.j;
}

List& List::operator=(const List& other) {
    this->j = other.j;
    return *this;
}

bool List::send(const InstanceId sender, const SocketWrapper& peer) {
    Serialized serialized = json::to_msgpack(j);

    if (!(peer << (uint8_t)MESSAGE_OPCODE))
        return false;

    if (!(peer << (uint8_t)LIST_MSG))
        return false;

    if (!(peer << serialized))
        return false;
        
    return true;
}
