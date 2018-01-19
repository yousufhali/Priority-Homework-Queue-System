//////////////////////////////////////////////////////////SYSTEM IMPLEMENTATION START////////////////////////////////////////////////////////////////
#include <iostream>
#include <limits.h>
using namespace std;


enum COURSE
{
	CHE102,
	MATH115,
	MATH117,
	ECE105,
	ECE150,
	ECE190,
	Null
};

struct Assignment
{
	COURSE course;       
	int    assnNum;      // ID: Assignment number
	int    dueMonth;     // Months from 1-12
	int    dueDay;       // Days from 1-31
	char*  description;  // description of assignment
};


							 
class HomeworkQueue 
{
public:
    bool                enqueue(const Assignment& assignment);
    const Assignment*   dequeue();
    int                 daysTillDue(const COURSE course);
    Assignment*   dueIn(const int numDays);


    HomeworkQueue();    //constructor
    ~HomeworkQueue();   //destructor
private:
	struct Homework{
        Assignment* assn;
        Homework *nextInQueue;
    };
    Homework* queue;
    Homework* nextQueue;
};

///Adding an assignment to the queue
bool HomeworkQueue::enqueue(const Assignment& assignment)
{
	int month=assignment.dueMonth;
	int day=assignment.dueDay;
	if (month<=0 || day<=0 || day>30 || month >12){
		return false;
	}
	if (*(assignment.description)==0 || assignment.description==0){
		return false;
	}
	if (assignment.course==Null){
		return false;
	}
	
	
	Assignment *toBeAdded=new Assignment;
	toBeAdded->course=assignment.course;
	toBeAdded->assnNum=assignment.assnNum;
	toBeAdded->dueMonth=assignment.dueMonth;
	toBeAdded->dueDay=assignment.dueDay;
	toBeAdded->description=assignment.description;
	
	Homework* newQueue=new Homework;
	
	newQueue->assn=toBeAdded;
	newQueue->nextInQueue=NULL;
	
	Homework* current=queue;
	
	if (current==NULL){
		
		queue=newQueue;
		
		return true;
		
	}
	
	while(current!=0 && current->assn->dueMonth<=assignment.dueMonth ){
	
		if (current->assn->course==assignment.course &&
			current->assn->dueDay==assignment.dueDay &&
			current->assn->dueMonth==assignment.dueMonth &&
			current->assn->assnNum==assignment.assnNum){
				return false;
			}
		if (current->nextInQueue==0)
			break;
		
		if (current->assn->dueMonth<assignment.dueMonth)
			current=current->nextInQueue;
		else if (current->assn->dueDay<=assignment.dueDay)
			current=current->nextInQueue;
		else
			break;
	}
	
	
	if (current!=0){
		newQueue->nextInQueue = current->nextInQueue;
	}
	if (current!=0){
	current->nextInQueue=newQueue;
	}
	
	
	
	return true;
}
//Removing assignment from queue
const Assignment* HomeworkQueue::dequeue()
{
	Homework* current=queue;
	
	if (current==0)
		return NULL;
	
	queue=queue->nextInQueue;
	
	return current->assn;
	
}
//Find the remaining days remaining for the earliest assignment for a given course
int HomeworkQueue::daysTillDue(const COURSE course)
{
	if (course!= CHE102 && course!=	MATH115 && course!=	MATH117 && course!= ECE105 && course!=ECE150 && course!=ECE190 && course != Null){
		return INT_MAX;
	}

	time_t timer;
	struct tm* timein;
	time (&timer);
	int months=0;
	int days=0;
	int daysTill=0;
	int totalDays=0;
	timein=localtime(&timer);
	months=timein->tm_mon;
	months+=1;
	days=timein->tm_mday;
	daysTill=(months*30)+days;
	Homework* current=queue;
	if (queue==NULL){
		return INT_MAX;
	}
	
	while (current){
		
		if (current->assn->course==course){
			if (current->assn->dueMonth <=12 && current->assn->dueMonth>=1 && current->assn->dueDay<=30 && current->assn->dueDay>=0 ){
			totalDays=((current->assn->dueMonth)*30)+(current->assn->dueDay);
			daysTill=totalDays-daysTill;;
			return daysTill;
			}
			else {
				return INT_MAX;
			}	
			
		}
		
		current=current->nextInQueue;		
	}
	return INT_MAX;
}
//Extract the assignments due in the next given number of days
Assignment* HomeworkQueue::dueIn(const int numDays)
{
	
	time_t timer;
	struct tm* timein;
	time (&timer);
	int mymonths=0;
	int mydays=0;
	int myTotalDays=0;
	int Total=0;
	int i=0;
	timein=localtime(&timer);
	mymonths=timein->tm_mon;
	mydays=timein->tm_mday;
	myTotalDays=(mymonths*30)+mydays;
	myTotalDays+=numDays;
	
	Assignment* assignmentArray=new Assignment[1000];

	Assignment newAssignment={
		.course=Null
	};
	i=0;
	
	while(i<1000){
		assignmentArray[i]=newAssignment;
		i++;
	}
	if (numDays<0){
		return assignmentArray;
	}
	i=0;
	Homework *queuer=queue;
	while (queuer!=0 && i<1000){
		Total=0;
		Total=(((queuer->assn->dueMonth)-1)*30)+(queuer->assn->dueDay);
		
		if (Total<=myTotalDays && queuer!=0){
				
				assignmentArray[i].assnNum=queuer->assn->assnNum;
				assignmentArray[i].course=queuer->assn->course;
				assignmentArray[i].dueDay=queuer->assn->dueDay;
				assignmentArray[i].dueMonth=queuer->assn->dueMonth;
				assignmentArray[i].description=queuer->assn->description;
				i++;
				
			}
		
		queuer=queuer->nextInQueue;

	}
	Assignment* returnAssignment=new Assignment[i];
	int z=0;
	while (assignmentArray[z].course!=Null){
		returnAssignment[z]=assignmentArray[z];
		z++;
	}
	returnAssignment[z].course=Null;
	return assignmentArray;
	
	
}

HomeworkQueue::HomeworkQueue()
{
	nextQueue=NULL;
    queue=NULL;
    
    
}

HomeworkQueue::~HomeworkQueue()
{
	//Destructor --- aka. cleaning up
	queue=NULL;
    nextQueue=NULL;
	
}
/////////////////////////////////////////////////////////////SYSTEM IMPLEMENTATION END//////////////////////////////////////////////////////////////////////////////
//This is a test implementation of the system
/////////////////////////////////////////////////TESTING///////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(const int argc, const char* const args[]) 
{
	
	HomeworkQueue hwq;

	char describeit[] = "This is a very important assignment for the course";

	Assignment someAssignment = 
	{
		.course = CHE102,
		.assnNum = 1000001,
		.dueMonth = 2,
		.dueDay = 2,
		.description = describeit
	};
	Assignment someAssignment2 = 
	{
		.course = ECE105,
		.assnNum = 1000001,
		.dueMonth = 3,
		.dueDay = 4,
		.description = describeit
	};
	Assignment someAssignment3 = 
	{
		.course = ECE105,
		.assnNum = 1000001,
		.dueMonth = 12,
		.dueDay = 4,
		.description = describeit
	};
	if(!hwq.enqueue(someAssignment))
	{
		cout << "The enqueue function needs to be checked --- Error enqueue()" << endl;
	}
	if(!hwq.enqueue(someAssignment3))
	{
		cout << "The enqueue function needs to be checked --- Error enqueue()" << endl;
	}
	if(!hwq.enqueue(someAssignment2))
	{
		cout << "The enqueue function needs to be checked --- Error enqueue()" << endl;
	}
	else
	{
		cout << "Test Result: Success" << std::endl;
	}

	
	hwq.dueIn(10);
	const Assignment* assnInTheQueue = hwq.dequeue();
	const Assignment* assnInTheQueue2 = hwq.dequeue();
	const Assignment* assnInTheQueue3 = hwq.dequeue();
	
	if (!assnInTheQueue3)
	{
		cout << "The dequeue function needs to be checked --- Error dequeue()" << endl;

	}
	else 
	{
		cout << "Test Result: Success" << endl;
	}

	

    return 0;
}
/////////////////////////////////////////////////////////////////////TESTING END/////////////////////////////////////////////////////////////////////////////