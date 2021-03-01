#include "../pch.h"
#include "CQBGM.h"
#include "CQSound.h"

CQBGM::CQBGM() {

}

CQBGM::~CQBGM() {

}
int CQBGM::LoopCount() {
	return XAUDIO2_LOOP_INFINITE;
}