#include <iostream>
#include <fstream>
#include <string>
#include <vector>
bool is_tag_name_letter(char c){
	return ('a'<=c && c<='z')||('A'<=c && c<='Z') || (c=='-') || (c=='_') || (c==':'); 
}
bool is_blank(char c){
	return (c==' ')||(c=='\t')||(c=='\n');
}




struct Tree
{	
	Tree(const std::string& name): tag(name){}
	std::string tag;
	std::vector<Tree*> children;
};



Tree* parse_tree(std::ifstream &xml){
	char c;
	while(xml.get(c) && c!='<'){
		;;
	}
	if (c != '<'){
		return 0;
	}
	if (c== '/'){
		//close tag;
		xml.get(c);
		return 0;
	}
	while (xml.get(c) && is_blank(c)){
		;;
	}
	
	if (!is_tag_name_letter(c)){
		return 0;
	}
	std::string left{c};
	while (xml.get(c) && is_tag_name_letter(c)) {
		left.push_back(c);
	}
	if (!left.length()){

		// std::cout<<" XML Parsing Error: XML declaration not well-formed\n";
		return 0;
	}
	Tree *root  = new Tree(left);
	while (xml){
		Tree *res = parse_tree(xml);
		if (!res){
			break;
		}
		root->children.push_back(res);

	}
	return root;

}
void print_tree(std::ostream& os, Tree* root, int tabs = 0){
	os<<std::string(tabs, '\t')<<root->tag<<"\n";
	for (Tree* child: root->children){
		print_tree(os, child, tabs+1);
	}
}

std::ostream &operator<<(std::ostream& os, Tree *root){
	print_tree(os, root);
	return os;
}

void print_help(){
	std::cout<<"Usage: xml-parser FILE\n";
}

int main(int argc, char *argv[]){
	if (argc<2){
		print_help();
		return 0;
	}
	std::ifstream xml (argv[1]);
	std::string token;
	xml>>token;
	if (token != "<?xml"){
		std::cout<<" xml syntax incorrect: header\n";
		return -1;
	}
	xml>>token;
	if (token != "version=\"1.0\""){
		std::cout<<" xml syntax incorrect: version\n";
		return -1;
	}
	char c;
	while(xml.get(c) && c!='?'){
		;;
	}
	if (c!='?'){
		std::cout<<" XML Parsing Error: XML declaration not well-formed\n";
		return -1;
	}
	if (xml.get(c) && c!='>'){
		std::cout<<" XML Parsing Error: XML declaration not well-formed\n";
		return -1;
	}
	auto root = parse_tree(xml);
	std::cout<<root;
	
	return 0;
}