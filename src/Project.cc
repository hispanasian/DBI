/*
 * Project.cc
 *
 *  Created on: Mar 13, 2015
 *      Author: cvasquez
 */

#include "Project.h"

Project::Project() {/* Do nothing */}

Project::~Project() {/* Do nothing */}

void Project::Use_n_Pages (int n) {/* Do nothing */}

void Project::Run (Pipe &inPipe, Pipe &outPipe, int *keepMe, int numAttsInput, int numAttsOutput) {
	ProjectData *data = new ProjectData { inPipe, outPipe, keepMe, numAttsInput, numAttsOutput, *this };

	thread_id = pthread_create(&worker, NULL, [] (void* args) -> void* {
		ProjectData *data = (ProjectData*)args;
		data->op.Work(data->in, data->out, data->keepMe, data->numAttsInput, data->numAttsOutput);
		delete data;
	}, (void*) data);
}

void Project::Work (Pipe &inPipe, Pipe &outPipe, int *keepMe, int numAttsInput, int numAttsOutput) {
	Record rec;
	Work(inPipe, outPipe, keepMe, numAttsInput, numAttsOutput, rec);
}

void Project::Work (Pipe &inPipe, Pipe &outPipe, int *keepMe, int numAttsInput, int numAttsOutput, Record &rec) {
	while(inPipe.Remove(&rec)) {
		rec.Project(keepMe, numAttsOutput, numAttsInput);
		outPipe.Insert(&rec);
	}

	// Important, must shut down outPipe
	outPipe.ShutDown();
}
