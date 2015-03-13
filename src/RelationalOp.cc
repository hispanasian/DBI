#include "../include/RelationalOp.h"

RelationalOp::RelationalOp() {
	thread_id = 0;
}

RelationalOp::~RelationalOp() {

}

void RelationalOp::WaitUntilDone () { (void) pthread_join(worker, NULL); }
