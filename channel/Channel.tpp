template <class T>
void Channel::kick(T it)
{
	this->_clientList.erase(it);
}
