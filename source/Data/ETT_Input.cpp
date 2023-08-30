#include "ETT_Input.h"
#include <numeric>


std::ostream& operator<<(std::ostream& os, const ETT_Input& in){
	// Insert the code that write the input object (needed for debugging purposes)
	int i,j;
	os << "[Exams:" << in.noExams << "]\n";
	for (i = 0; i < in.noExams; ++i) {
		os << in.examDuration[i];
		for(j=0; j < in.examStudents[i].size(); ++j){
			os << ", " <<  in.examStudents[i][j];
		}
		os << std::endl;
	}
	os << "[Periods:" << in.noPeriods << "]" << std::endl;
	for(i = 0; i < in.noPeriods; ++i){
		os << in.periodDayName[i] << ", " << in.periodSlotName[i] << ", " << in.periodDuration[i] << ", " << in.periodPenalty[i] << std::endl;
	}
	os << "[Rooms:" << in.noRooms << "]" << std::endl;
	for(i=0; i < in.noRooms; ++i){
		os << in.roomCapacity[i] << ", " << in.roomPenalty[i] << std::endl;
	}
	os << "[PeriodHardConstraints]" << std::endl;
	for(i=0; i < in.noExams; ++i){
		for(j=0; j < in.examAfter[i].size(); ++j){
			os << i << ", AFTER, " << in.examAfter[i][j] << std::endl;
		}
	}
	for(i=0; i < in.noExams; ++i){
		for(j=0; j < in.examExcl[i].size(); ++j){
			if(i < in.examExcl[i][j]){
				os << i << ", EXCLUSION, " << in.examExcl[i][j] << std::endl;
			}
		}
	}
	for(i=0; i < in.noExams; ++i){
		for(j=0; j < in.examCoin[i].size(); ++j){
			if(i < in.examCoin[i][j]){
				os << i << ", EXAM_COINCIDENCE, " << in.examCoin[i][j] << std::endl;
			}
		}
	}
	os << "[RoomHardConstraints]" << std::endl;
	for(i=0; i < in.noExams; ++i){
		if(in.examAlone[i]){
			os << i << ", ROOM_EXCLUSIVE" << std::endl;
		}
	}
	os << "[InstitutionalWeightings]" << std::endl;
	os << "TWOINAROW, " << in.twoInARowCost << std::endl;
	os << "TWOINADAY, " << in.twoInADayCost << std::endl;
	os << "PERIODSPREAD, " << in.periodSpread << std::endl;
	os << "NONMIXEDDURATIONS, " << in.nonMixedDurationsCost << std::endl;
	os << "FRONTLOAD, " << in.getFrontLoadSize() << ", " << (in.getNoPeriods() - in.getFirstFrontLoadPeriod()) << ", " << in.getFrontLoadCost() << std::endl;
	return os;
}

ETT_Input::ETT_Input(std::string fileName, const std::vector<int>& hardConWeights) :  ROOM_OCCUPANCY_COST(hardConWeights[0]), PERIOD_UTILIZATION_COST(hardConWeights[1]), PERIOD_RELATED_COST(hardConWeights[2]), ROOM_RELATED_COST(hardConWeights[3]){
	PHC = 0; 	
	RHC = 0; 	
	FLP = 0; 	
	CD = 0; 	
	ExR = 0; 	
	SxE = 0; 	
	SCap = 0; 	
	PC = 0; 	

	std::string word;
	std::ifstream file(fileName);
	if(file.is_open()){
		int i,j,id;

		// No. exams: [Exams:<examNo>]
		file.ignore(7,':');
		file >> noExams;
		file.ignore(1);
	
		// For each exam: <duration>, <studentId>, ..., <studentId>
		noStudents = 0;
		examDuration.resize(noExams);
		examStudents.resize(noExams);
		for(i=0; i < noExams; ++i){
			file >> examDuration[i];
			while(file.peek() == ','){
				file.ignore();
				file >> id;
				examStudents[i].push_back(id);

				// Store all unique student ids
				// For each students, store exam enrollments
				for(j=0; j < noStudents; ++j){
					if(studentIds[j] == id){ // Existing student
						studentExams[j].push_back(i);
						break;
					}
				}
				if(j==noStudents){ // New student
					noStudents++;
					studentIds.push_back(id);
					studentExams.push_back({i});
				}
				SxE++;
				SCap++;
			}
		}
		file.ignore(1);		

		// Sort examStudent vectors (speeds up computation conflict graph + pretty printing)
		for(i=0; i < noExams; ++i){
			std::sort(examStudents[i].begin(), examStudents[i].end());
		}

		// Construct the conflict graph by counting for each pair 
		// of exams how many students they have in common
		int noCommon;
		std::vector<int> dummy;
		conflict.resize(noExams, std::vector<int>(noExams, 0));
		conflictList.resize(noExams);
		for(i=0; i < noExams; ++i){
			for(j=i+1; j < noExams; ++j){ // For each pair of exams
				dummy.clear();
				std::set_intersection(examStudents[i].begin(), examStudents[i].end(),
						examStudents[j].begin(), examStudents[j].end(), 
						std::inserter(dummy,dummy.begin()));
				noCommon = dummy.size();
				if(noCommon > 0){
					conflict[i][j] = dummy.size();
					conflict[j][i] = dummy.size();
					conflictList[i].push_back(j);
					conflictList[j].push_back(i);
					CD += 2;
				}
			}
		}

		// Store the pair conflict list
		pairConflictList.resize(noExams, std::vector<std::vector<int>>(noExams, std::vector<int>()));
		for(i=0; i < noExams; ++i){
			for(j=i+1; j < noExams; ++j){ // For each pair of exams
				pairConflictList[i][j].reserve(conflictList[i].size() + conflictList[j].size());
				std::set_union(conflictList[i].begin(), conflictList[i].end(), conflictList[j].begin(), conflictList[j].end(), std::back_inserter(pairConflictList[i][j]));
				pairConflictList[j][i] = pairConflictList[i][j];
			}
		}
		

		// No. periods: [Periods:<periodNo>]

		file.ignore(9,':');
		file >> noPeriods;
		file.ignore(1);		
		
		// For each period: <day>:<month>:<year>, <hour>:<minute>:<second>, <duration>, <penalty>
		periodDay.resize(noPeriods);
		periodSlot.resize(noPeriods);
		periodDuration.resize(noPeriods);
		periodPenalty.resize(noPeriods);
		periodDayName.resize(noPeriods);
		periodSlotName.resize(noPeriods);
		int day, month, year, hour, minute, second, slot=0;
		int firstDay[3], currentDay[3];
		for(i=0; i < noPeriods; ++i){
			file >> day;
			file.ignore(1);
			file >> month;
			file.ignore(1);
			file >> year;
			file.ignore(1);
			file >> hour;
			file.ignore(1);
			file >> minute;
			file.ignore(1);
			file >> second;
			file.ignore(1);
			file >> periodDuration[i];
			file.ignore(1);
			file >> periodPenalty[i];
			file.ignore(1);

			periodDayName[i] = std::to_string(day) + ":" + std::to_string(month) + ":" + std::to_string(year);
			periodSlotName[i] = std::to_string(hour) + ":" + std::to_string(minute) + ":" + std::to_string(second);

			// Store the first day in the examination period
			// Index of a day corresponds to the number of days since the start of the examination period
			if(i == 0){
				firstDay[0] = day;
				firstDay[1] = month;
				firstDay[2] = year;
				slot = 0;
			}

			currentDay[0] = day;
			currentDay[1] = month;
			currentDay[2] = year;

			// Number of days since the start if the examination period
			periodDay[i] = countNoOfDays(firstDay, currentDay);

			if(i != 0 && periodDay[i] == periodDay[i-1]){
				slot++;
			} else {
				slot = 0;
			}

			periodSlot[i] = slot;
		}

                // Determine the maximal number of days in a slot
                maxPeriodsDay = 0;
                int prevDay = -1;
                int cntr = 0;
                for(int i=0; i < noPeriods; ++i){
                        day = getPeriodDay(i);  
                        if(day == prevDay){
                                cntr++;
                        } else {
                                if(cntr > maxPeriodsDay){
                                        maxPeriodsDay = cntr + 1;
                                }
                                cntr = 0;
                        }
                        prevDay = day;
                }

		// No. rooms: [Rooms:<roomNo>]
		file.ignore(10,':');
		file >> noRooms;
		file.ignore(1);

		// For each room: <capacity>, <penalty>
		roomCapacity.resize(noRooms);
		roomPenalty.resize(noRooms);
		for (i = 0; i < noRooms; ++i) {
			file >> roomCapacity[i];
			file.ignore(1);
			file >> roomPenalty[i];
		}

        	// [PeriodHardConstraints]
        	// <examId1>, <precedenceType>, <examId2>
		char precedenceRelation;
		examBefore.resize(noExams);
		examAfter.resize(noExams);
		examExcl.resize(noExams);
		examCoin.resize(noExams);
        	file.ignore(100,']');
		file.get(); 
        	while(file.peek()!='[') {
			file >> i;
			file.ignore(4);
			file >> precedenceRelation;
                	file.ignore(15,' ');
			file >> j;
			if(i != j){
				getline(file, word);
				switch (precedenceRelation) {
					case 'T':
						examAfter[i].push_back(j);
						examBefore[j].push_back(i);
						break;
					case 'C':
						examExcl[i].push_back(j);
						examExcl[j].push_back(i);
						break;
					case 'A':
						examCoin[i].push_back(j);
						examCoin[j].push_back(i);
						break;
					default:
						throw std::logic_error("Case " + std::to_string(precedenceRelation) + " not expected");
				}
			}
			PHC++;
		}
        	getline(file,word);

		// [RoomHardConstraints]
        	// <roomId>, ROOM_EXCLUSIVE
		examAlone.resize(noExams, 0);
        	while(file.peek()!='['){
                	file >> i;
			examAlone[i] = true;
			getline(file, word);
			RHC++;
        	}
        	getline(file,word);

		// [InstitutionalWeightings]
		file.ignore(200,',');
		file >> twoInARowCost;
		file.ignore(200,',');
		file >> twoInADayCost;
		file.ignore(200,',');
		file >> periodSpread;
		file.ignore(200,',');
		file >> nonMixedDurationsCost;
		file.ignore(200,',');
		int noFrontLoadExams;
		file >> noFrontLoadExams;
	       	file.ignore(1);
		int noFrontLoadPeriods;
		file >> noFrontLoadPeriods;
		FLP = noFrontLoadPeriods;
		firstFrontLoadPeriod = std::max(noPeriods - noFrontLoadPeriods,0);
	       	file.ignore(1);
		file >> frontLoadCost;
	       	file.ignore(1);

		// Determine frontLoad (sort first by exam size, then by appearance in input file)
		// Create auxiliary vector of examIndices, then sort this vector based on no students
		std::vector<int> examIndices(getNoExams());
		std::iota(examIndices.begin(), examIndices.end(), 0);
		std::sort(examIndices.begin(), examIndices.end(), 
				[&](int i, int j){return getNoExamStudents(i) > getNoExamStudents(j) || (getNoExamStudents(i) == getNoExamStudents(j) && i < j);} );

		// frontLoadCost[0] largest exams are frontLoad
		frontLoad.resize(getNoExams(), false);
		frontLoadIndices.reserve(noFrontLoadExams);
		for(i=0; i < noFrontLoadExams; ++i){
			frontLoad[examIndices[i]] = true;
			frontLoadIndices.push_back(examIndices[i]);
		}

		// Determine validPeriod
		validPeriod.resize(noExams, std::vector<bool>(noPeriods, true));
		validPeriodList.resize(noExams);
		validPeriodIndex.resize(noExams, std::vector<int>(noPeriods, -1));
		for(i=0; i < noExams; ++i){
			for(j=0; j < noPeriods; ++j){
				if(examDuration[i] > periodDuration[j]){
					validPeriod[i][j] = false;
				} else {
					validPeriodList[i].push_back(j);
					validPeriodIndex[i][j] = validPeriodList[i].size()-1;
				}
			}
		}

		// Determine validRoom
		validRoom.resize(noExams, std::vector<bool>(noRooms, true));
		validRoomList.resize(noExams);
		validRoomIndex.resize(noExams, std::vector<int>(noRooms, -1));
		for(i=0; i < noExams; ++i){
			for(j=0; j < noRooms; ++j){
				if(getNoExamStudents(i) > roomCapacity[j]){
					validRoom[i][j] = false;
				} else {
					validRoomList[i].push_back(j);
					validRoomIndex[i][j] = validRoomList[i].size()-1;
				}
			}
			if(getNoValidRooms(i) < 1){
				throw std::logic_error("Instance infeasible. Expected at least one valid room for exam " + std::to_string(i));
			}
		}

		// Store all unique exam durations (for calculation mixedDurations cost)
		int dur;
		uniqueExamDurations = {};
		examDurationIndex.resize(noExams);
		for(i=0; i < noExams; ++i){
			dur = examDuration[i];
			for(j=0; j < uniqueExamDurations.size() && uniqueExamDurations[j] != dur; ++j){
			}
			if(j == uniqueExamDurations.size()){ // Duration was not found
				uniqueExamDurations.push_back(dur);
			}
			examDurationIndex[i] = j;
		}

		// Enumerate all connected components
		findComponents();

	} else {
		throw std::logic_error("Unable to open file " + fileName);
	}


	int Nps=0, Nsd=0, Nir=0;
	for(int p1=0; p1 < getNoPeriods(); ++p1){
		for(int p2=p1+1; p2 < getNoPeriods(); ++p2){
			if(p2-p1 <= getPeriodSpread()){
				Nps++;
			} 
			if(getPeriodDay(p1) == getPeriodDay(p2)){
				if(p2-p1==1) Nir++;
				else Nsd++;
			}
		}
	}

	CD = (CD+0.0)/(noExams*noExams+0.0);
	ExR = (noExams+0.0)/(noRooms*noPeriods+0.0); 	
	SxE = (SxE+0.0)/(noExams+0.0); 	
	SCap = (SCap+0.0)/(std::accumulate(roomCapacity.begin(), roomCapacity.end(), 0)*noPeriods+0.0); 	
	PC = (Nps*2 + ((Nsd*2)*twoInADayCost)+ ((Nir*2)*twoInARowCost))/(noPeriods + 0.0);
	
	std::cout << "Features: " << getNoExams() << " " << getNoStudents() << " " << getNoPeriods() << " " << getNoRooms() << " " << PHC << " " << RHC << " " << FLP << " " << CD << " " << ExR << " " << SxE << " " << SCap << " " << PC << std::endl;


#ifdef VISUALIZE
	visualize();
#endif
}

int ETT_Input::merge(int* parent, int x) {
	if (parent[x] == x){
		return x;
	}
	return merge(parent, parent[x]);
}


// Ignore edges with less than THRESHOLD students in common
#define THRESHOLD 0
#define MINCOMP 2 	// Minimum size of the component
#define MAXCOMP 50	// Maximum size of the component
			//
	
int ETT_Input::findComponents() {
	// See https://www.geeksforgeeks.org/connected-components-in-an-undirected-graph/?ref=rp
	
	// Store for each exam its parent component
	int parent[noExams];
	int i,j;
	for (i = 0; i < noExams; i++) {
		parent[i] = i;
	}
	for (i=0; i < noExams-1; ++i){
		for (j = i+1; j < noExams; ++j) {
			if(conflict[i][j] > THRESHOLD){
				parent[merge(parent, i)] = merge(parent, j);
			}	
		}
	}

	for (i = 0; i < noExams; i++) {
		parent[i] = merge(parent, parent[i]);
	}

	std::map<int, std::list<int> > m;
	for (i = 0; i < noExams; i++) {
		m[parent[i]].push_back(i);
	}

	components = {};
	componentsSet = {};
	for (auto it = m.begin(); it != m.end(); it++) {
		if (it->second.size() >= MINCOMP && it->second.size() <= MAXCOMP) {
			components.push_back(std::vector<int>(it->second.begin(), it->second.end()));
			componentsSet.push_back(std::set<int>(it->second.begin(), it->second.end()));
		}
	}
	noComponents = components.size();

	// Sort the exams in each component in decreasing size of students
	// Needed in the best fit algorithm
	for(auto &component : components){
		std::sort(component.begin(), component.end(), [this](int l, int r) {return getNoExamStudents(l) > getNoExamStudents(r);});
	}

#ifndef NDEBUG
	std::cout << "===========================" << std::endl;
	std::cout << "Components: " << components.size() << std::endl;
	for(auto& component : components){
		for(auto& c : component){
			std::cout << c << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "===========================" << std::endl;
#endif


	return noComponents;
}

int ETT_Input::countLeapYearDays(const int d[]) const{
	int years = d[2];
	if (d[1] <= 2){
		years--;
	}
	return ( (years / 4) - (years / 100) + (years / 400) );
}

int ETT_Input::countNoOfDays(const int date1[], const int date2[]) const{
	// Count the number of days since 00/00/00 for each of the two dates
	// Substract the difference
	long int dayCount1 = (date1[2] * 365);
	dayCount1 += countLeapYearDays(date1);
	dayCount1 += MONTH_DAYS[date1[1]-1];
	dayCount1 += date1[0];

	long int dayCount2 = (date2[2] * 365);
	dayCount2 += MONTH_DAYS[date2[1]-1];
	dayCount2 += date2[0];
	dayCount2 += countLeapYearDays(date2);

	return ( abs(dayCount1 - dayCount2) );
}

void ETT_Input::visualize(){
	std::cout << "Start" << std::endl;
	std::ofstream nodefile;
	nodefile.open ("nodes.dot");
	nodefile << "graph {node [margin=0 fontsize=14 width=0.2 shape=circle]" << std::endl;
	for (int e1 = 0; e1 < noExams; ++e1) {
		nodefile << '"' + std::to_string(e1+1) + "\" [label=\"" + std::to_string(e1+1) + "\"];" << std::endl;
	}
	for (int e1 = 0; e1 < noExams; ++e1) {
		for (int e2 = e1+1; e2 < noExams; ++e2) {
			if (getConflict(e1,e2) > THRESHOLD) {
				nodefile << "\"" + std::to_string(e1+1) + "\" -- \"" + std::to_string(e2+1) + "\" [label= \"" + std::to_string(getConflict(e1,e2)) + "\" ];" << std::endl;
			} 
			//else if (getConflict(e1,e2) > 0){
			//	nodefile << "\"" + std::to_string(e1+1) + "\" -- \"" + std::to_string(e2+1) + "\" [label= \"" + std::to_string(getConflict(e1,e2)) + "\" color=\"red\" ];" << std::endl;
			//}
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
