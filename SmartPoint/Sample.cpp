#include<iostream>
#include<vector>

struct Node
{
	int iVal;
};

using S_Node = std::shared_ptr<Node>;


void main()
{
	std::vector<S_Node> list;
	list.push_back(S_Node(new Node));
	auto data = list[0];
	std::cout << data.get()->iVal << std::endl;
}