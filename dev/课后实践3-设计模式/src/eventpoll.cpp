#include "../include/eventpoll.hpp"

EventPoll::EventPoll(std::shared_ptr<Lidar> l):_l(l){}
EventPoll::~EventPoll(){}

std::vector<std::string> EventPoll::wait(){
    return _l->getStatus();
}