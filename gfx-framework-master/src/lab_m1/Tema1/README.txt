Apostol Stefan Alexandru, 334CA

Pentru implementarea jocului s-au folosit structuri de date
pentru rata, vieti, gloante, componente geometrice simple
si date de organizare a jocului precum eventManager, gameMode,
gameState, duckState.

S-au implementat urmatoarele aspecte legate de gameplay din jocul
original :
	- sistem de vieti
	- sistem de nivele
	- sistem de munitie
	- scor si scor maxim
	- rata care zboara pe ecran
	- ciocnire rata cu peretii ecranului
	- sitem tragere si coliziuni dintre glont si rata
	- animatie de evadare si moarte rata
	- temporizare ca sa apara doar 1 rata pe ecran

Aspecte extra implementate fata de cele din jocul original:
	- mai multe arme (awp cu sistem diferit de amunitie)
	- scor multiplier cand se foloseste awp
	- headshot system
	- eventManager (crestere viteza, miscorare rata, "rata prietenoasa", 
	bounce aleator)
	- crestere sansa trigger la evenimente cand se trece la alt nivel
	- scope mode
