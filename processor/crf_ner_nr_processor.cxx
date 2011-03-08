/*
 * Copyright (c) 2008, weibingzheng@gmail.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY weibingzheng@gmail.com ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL detrox@gmail.com BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#include <cassert>
#include <cstdio>
#include <stdexcept>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include "crf_ner_nr_processor.hxx"

using namespace std;

namespace bamboo {


PROCESSOR_MAGIC
PROCESSOR_MODULE(CRFNRProcessor)

CRFNRProcessor::CRFNRProcessor(IConfig *config)
	:_ner_type("nr"), _ner_output_type(0)
{
	const char * model;
	struct stat buf;
	config->get_value("crf_ner_nr_model", model);

#ifdef DEBUG
	std::string model_param = std::string("-n5 -m ") + std::string(model);
#else
	std::string model_param = std::string("-m ") + std::string(model);
#endif

	if(stat(model, &buf)==0) {
		_tagger = CRFPP::createTagger(model_param.c_str());
	} else {
		throw std::runtime_error(std::string("can not load model ") + model + ": " + strerror(errno));
	}
	config->get_value("ner_output_type", _ner_output_type);

}

CRFNRProcessor::~CRFNRProcessor() {
	delete _tagger;
}

void CRFNRProcessor::process(std::vector<TokenImpl *> &in, std::vector<TokenImpl *> &out) {
	_tagger->clear();

	size_t i, size = in.size(), max_token_size = 0;

	for(i=0; i<size; ++i) {
		TokenImpl *token = in[i];
		const char * str = token->get_token();
		_tagger->add(1, &str);
		max_token_size += token->get_bytes();
	}

	if (!_tagger->parse()) throw std::runtime_error("crf parse failed!");

	std::string ner_str(""), ner_str_orig("");
	ner_str.reserve(max_token_size);
	ner_str_orig.reserve(max_token_size);
	assert(in.size() == _tagger->size());
	size = _tagger->size();
	for(i=0; i<size; ++i) {
		const char *tag = _tagger->y2(i);
		TokenImpl *token = in[i];
		if(*tag == 'O') {
			if(ner_str.size() > 0) {
				out.push_back(new TokenImpl(ner_str.c_str(), ner_str_orig.c_str()));
				out.back()->set_pos(_ner_type);
				ner_str.clear();
				ner_str_orig.clear();
			}
			if(_ner_output_type==1)
				out.push_back(new TokenImpl(*token));
		} else {
			ner_str += token->get_token();
			ner_str_orig += token->get_orig_token();
			if( (*tag=='E'||*tag=='S') && ner_str.size()>0) {
				out.push_back(new TokenImpl(ner_str.c_str(), ner_str_orig.c_str()));
				out.back()->set_pos(_ner_type);
				ner_str.clear();
				ner_str_orig.clear();
			}
		}
		delete token;
	}
	if(ner_str.size() > 0) {
		out.push_back(new TokenImpl(ner_str.c_str(), ner_str_orig.c_str()));
		out.back()->set_pos(_ner_type);
		ner_str.clear();
		ner_str_orig.clear();
	}
}


} //namespace bamboo
