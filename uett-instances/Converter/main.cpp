#include <iostream>
#include <fstream>
#include <string>
#include <vector>

std::vector<std::string> dayYear = {
	"01:01:1900, 08:00:00, 1, 0",
	"02:01:1900, 08:00:00, 1, 0",
	"03:01:1900, 08:00:00, 1, 0",
	"04:01:1900, 08:00:00, 1, 0",
	"05:01:1900, 08:00:00, 1, 0",
	"06:01:1900, 08:00:00, 1, 0",
	"07:01:1900, 08:00:00, 1, 0",
	"08:01:1900, 08:00:00, 1, 0",
	"09:01:1900, 08:00:00, 1, 0",
	"10:01:1900, 08:00:00, 1, 0",
	"11:01:1900, 08:00:00, 1, 0",
	"12:01:1900, 08:00:00, 1, 0",
	"13:01:1900, 08:00:00, 1, 0",
	"14:01:1900, 08:00:00, 1, 0",
	"15:01:1900, 08:00:00, 1, 0",
	"16:01:1900, 08:00:00, 1, 0",
	"17:01:1900, 08:00:00, 1, 0",
	"18:01:1900, 08:00:00, 1, 0",
	"19:01:1900, 08:00:00, 1, 0",
	"20:01:1900, 08:00:00, 1, 0",
	"21:01:1900, 08:00:00, 1, 0",
	"22:01:1900, 08:00:00, 1, 0",
	"23:01:1900, 08:00:00, 1, 0",
	"24:01:1900, 08:00:00, 1, 0",
	"25:01:1900, 08:00:00, 1, 0",
	"26:01:1900, 08:00:00, 1, 0",
	"27:01:1900, 08:00:00, 1, 0",
	"28:01:1900, 08:00:00, 1, 0",
	"29:01:1900, 08:00:00, 1, 0",
	"30:01:1900, 08:00:00, 1, 0",
	"31:01:1900, 08:00:00, 1, 0",
	"01:02:1900, 08:00:00, 1, 0",
	"02:02:1900, 08:00:00, 1, 0",
	"03:02:1900, 08:00:00, 1, 0",
	"04:02:1900, 08:00:00, 1, 0",
	"05:02:1900, 08:00:00, 1, 0",
	"06:02:1900, 08:00:00, 1, 0",
	"07:02:1900, 08:00:00, 1, 0",
	"08:02:1900, 08:00:00, 1, 0",
	"09:02:1900, 08:00:00, 1, 0",
	"10:02:1900, 08:00:00, 1, 0",
	"11:02:1900, 08:00:00, 1, 0",
	"12:02:1900, 08:00:00, 1, 0",
	"13:02:1900, 08:00:00, 1, 0",
	"14:02:1900, 08:00:00, 1, 0",
	"15:02:1900, 08:00:00, 1, 0",
	"16:02:1900, 08:00:00, 1, 0",
	"17:02:1900, 08:00:00, 1, 0",
	"18:02:1900, 08:00:00, 1, 0",
	"19:02:1900, 08:00:00, 1, 0",
	"20:02:1900, 08:00:00, 1, 0",
	"21:02:1900, 08:00:00, 1, 0",
	"22:02:1900, 08:00:00, 1, 0",
	"23:02:1900, 08:00:00, 1, 0",
	"24:02:1900, 08:00:00, 1, 0",
	"25:02:1900, 08:00:00, 1, 0",
	"26:02:1900, 08:00:00, 1, 0",
	"27:02:1900, 08:00:00, 1, 0",
	"28:02:1900, 08:00:00, 1, 0",
	"01:03:1900, 08:00:00, 1, 0",
	"02:03:1900, 08:00:00, 1, 0",
	"03:03:1900, 08:00:00, 1, 0",
	"04:03:1900, 08:00:00, 1, 0",
	"05:03:1900, 08:00:00, 1, 0",
	"06:03:1900, 08:00:00, 1, 0",
	"07:03:1900, 08:00:00, 1, 0",
	"08:03:1900, 08:00:00, 1, 0",
	"09:03:1900, 08:00:00, 1, 0",
	"10:03:1900, 08:00:00, 1, 0",
	"11:03:1900, 08:00:00, 1, 0",
	"12:03:1900, 08:00:00, 1, 0",
	"13:03:1900, 08:00:00, 1, 0",
	"14:03:1900, 08:00:00, 1, 0",
	"15:03:1900, 08:00:00, 1, 0",
	"16:03:1900, 08:00:00, 1, 0",
	"17:03:1900, 08:00:00, 1, 0",
	"18:03:1900, 08:00:00, 1, 0",
	"19:03:1900, 08:00:00, 1, 0",
	"20:03:1900, 08:00:00, 1, 0",
	"21:03:1900, 08:00:00, 1, 0",
	"22:03:1900, 08:00:00, 1, 0",
	"23:03:1900, 08:00:00, 1, 0",
	"24:03:1900, 08:00:00, 1, 0",
	"25:03:1900, 08:00:00, 1, 0",
	"26:03:1900, 08:00:00, 1, 0",
	"27:03:1900, 08:00:00, 1, 0",
	"28:03:1900, 08:00:00, 1, 0",
	"29:03:1900, 08:00:00, 1, 0",
	"30:03:1900, 08:00:00, 1, 0",
};


int main(int argc, char *argv[])
{
	std::string word;
	if (argc < 3) {
		std::cout << "Please provide filename: ./a.out inputFilename outputFile" << std::endl;
	}
	std::string fileName = argv[1];
	std::ifstream file(fileName);
	std::vector<std::vector<int>> examEnrollments;
	std::vector<int> examSize;
	int nrExams = -1;
	int nrStudents = -1;
	int nrPeriods = -1;
	int totExamEnrollments = 0;
	if(file.is_open()){
		file >> nrExams;
		examEnrollments.resize(nrExams, {});
		examSize.resize(nrExams);

		file >> nrStudents;

		file >> nrPeriods;
		if(nrPeriods > dayYear.size()){
			std::cout << "Not enough days in dayYear" << std::endl;
			std::abort();
		}

		// Read all exam sizes
		int exam, size;
		for (int i = 0; i < nrExams; ++i) {
			file >> exam;
			file >> size;
			if(exam != i+1){
				std::cout << "Exam mismatch" << std::endl;
				std::cout << "Exam: " << exam << std::endl;
				std::cout << "i: " << i << std::endl;
				std::abort();
			}
			examSize[i] = size;
			totExamEnrollments += size;
		}
		std::string student;
		for (int i = 0; i < totExamEnrollments; ++i) {
			file >> student;
			file >> exam;
			student.erase(0,1);
			examEnrollments[exam-1].push_back(std::stoi(student));
		}
	}
	file.close();

	// Write file into new instance file
	std::ofstream outputFile;
	std::string output = argv[2];
	std::cout << output << std::endl;
	outputFile.open(output.c_str());
	outputFile << "[Exams:" << nrExams << "]\n";
	for (int i = 0; i < nrExams; ++i) {
		outputFile << "1";
		for (auto k : examEnrollments[i]) {
			outputFile << ", " << k;
		}
		outputFile << "\n";
	}

	outputFile << "[Periods:" << nrPeriods << "]\n";
	for (int i = 0; i < nrPeriods; ++i) {
		outputFile << dayYear[i] << "\n";	
	}

	outputFile << "[Rooms:1]\n";
	outputFile << totExamEnrollments << ", 0\n";
	outputFile << "[RoomHardConstraints]\n";
	outputFile << "[PeriodHardConstraints]\n";
	outputFile << "[InstitutionalWeightings]\n";
	outputFile << "TWOINAROW, 0\n";
	outputFile << "TWOINADAY, 0\n";
	outputFile << "PERIODSPREAD, 5\n";
	outputFile << "NONMIXEDDURATIONS, 0\n";
	outputFile << "FRONTLOAD, 0, 0, 0\n";


	outputFile.close();



	return 0;
}
