#include "ETT_Output.h"

ETT_Output::ETT_Output(const ETT_Input& my_in): in(my_in), roomAssignment(in.getNoExams(), 0), periodAssignment(in.getNoExams(), 0){
	// Insert the code that initialize the data structures of the
	// output object based in the input object
}

ETT_Output& ETT_Output::operator=(const ETT_Output& out) {
	// Insert the code that copies all data structures of the
	// output object from the ones of the parameter out
	// (excluding the reference to the input object, that is constant)
	
	periodAssignment = std::vector<int>(out.periodAssignment);
	roomAssignment = std::vector<int>(out.roomAssignment);
	
	return *this;
}

std::ostream& operator<<(std::ostream& os, const ETT_Output& out) { 
	// Insert the code that writes the output object
	int i;
	for (i = 0; i < out.periodAssignment.size(); ++i) {
		os << out.periodAssignment[i] << ", " << out.roomAssignment[i] << "\r\n";
	}
	return os;
}

std::istream& operator>>(std::istream& is, ETT_Output& out) {
	// Insert the code that reads the output object
	int i;
	char buffer[256];

	// Read the output of a state print
	for(i=0; i < (int) out.periodAssignment.size(); ++i){
		is >> out.periodAssignment[i];
		is >> buffer;
		is >> out.roomAssignment[i];
	}

	//out.visualize();

	return is;
}

void ETT_Output::visualize(){
	std::cout << "Start visualization" << std::endl;
	std::ofstream nodefile;
	nodefile.open ("nodes.dot");
	nodefile << "graph {node [margin=0 fontsize=14 width=0.2 shape=circle]" << std::endl;
	for (int e1 = 0; e1 < in.getNoExams(); ++e1) {
		nodefile << '"' + std::to_string(e1+1) + "\" [label=\"" + std::to_string(e1+1) + "\"];" << std::endl;
	}
	for (int e1 = 0; e1 < in.getNoExams(); ++e1) {
		for (int e2 = e1+1; e2 < in.getNoExams(); ++e2) {
			if (in.getConflict(e1,e2) > 0 && std::abs(periodAssignment[e1] - periodAssignment[e2]) <= 2*in.getPeriodSpread()) {
				nodefile << "\"" + std::to_string(e1+1) + "\" -- \"" + std::to_string(e2+1) + "\" [label= \"" + std::to_string(in.getConflict(e1,e2)) + "\" ];" << std::endl;
			}
		}
	}
	nodefile << "}" << std::endl;
	nodefile.close();
	// Erase any trailing zeros
	//std::string command = "dot -Tps nodes.dot -Tpdf > temp.pdf";
	std::string command = "sfdp -Tps nodes.dot -Tpdf > temp.pdf";
	std::cout << command << std::endl;
	system(command.c_str());
	std::cout << "Done" << std::endl;
}
