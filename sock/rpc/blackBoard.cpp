#include "blackBoard.h"
//FIXME -- move globals to header?
vector<knowledgeItem *> * knowledgeItems = new vector<knowledgeItem *>;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

map<uint32_t, knowledgeItem *> tagMap;

bbThread * threadManager;

int main()
{
	threadManager = new bbThread();

	//initialize some things
	pthread_t ptUI, ptCS, ptKS;

	//todo: move this?
	
	
	knowledgeItem * e;

	e = new knowledgeItem();
	e->id = KI_A;
	knowledgeItems->push_back(e);
	e = new knowledgeItem();
	e->id = KI_B;
	knowledgeItems->push_back(e);
	e = new knowledgeItem();
	e->id = KI_C;
	knowledgeItems->push_back(e);
	e = new knowledgeItem();
	e->id = KI_D;
	knowledgeItems->push_back(e);
	
	printf("☢CAUTION!!☢\n");
	
	//start the interface thread
	printf("starting UI thread...");
	pthread_create(&ptUI, NULL, runUI, (void *)NULL);
	pthread_create(&ptCS, NULL, runCSServer, (void *)NULL);
	pthread_create(&ptKS, NULL, runKSServer, (void *)NULL);
	printf("\tdone!\n");

	
	pthread_join(ptUI, NULL);
	pthread_join(ptCS, NULL);
	pthread_join(ptKS, NULL);
}


