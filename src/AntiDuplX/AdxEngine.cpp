/*
* AntiDuplX utility (http://github.com/ermig1979/AntiDuplX).
*
* Copyright (c) 2023-2023 Yermalayeu Ihar.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#include "AntiDuplX/AdxEngine.h"

namespace Adx
{
	Engine::Engine(const Options& options)
		: _options(options)
		, _imageFinder(options, _imageInfos)
		, _imageLoader(options, _imageInfos)
		, _imageMatcher(options, _imageInfos)
		, _resultHandler(options, _imageInfos, _imagePaths)
	{
	}

	Engine::~Engine()
	{
		for (size_t i = 0; i < _imageInfos.size(); ++i)
			delete _imageInfos[i];
		_imageInfos.clear();

		
	}

	bool Engine::LoadDups() {
		fs::path path(_options.dupFile);
		std::ifstream ifs(_options.dupFile.c_str());
    if (!ifs.is_open()) {
      CPL_LOG_SS(Error, "Can't open duplicates file '" << path.string() << "'!");
      return false;
    }

		String dup1, dup2;
		while(getline(ifs, dup1) && getline(ifs, dup2)) {
			if (dup1.empty() || dup2.empty()) {
				CPL_LOG_SS(Error, "Unexpected formatting in duplicates file.");
				return false;
			}

			_imagePaths.insert(make_pair(dup1, dup2));			

			getline(ifs, dup2);
			if (! dup2.empty()) {
				CPL_LOG_SS(Error, "Unexpected formatting in duplicates file.");
				return false;
			}
		}

		return true;
	}

	bool Engine::Run()
	{
		CPL_PERF_FUNC();

		if (!LoadDups())
			return false;

		if (!_imageFinder.Run())
			return false;

		if (!_imageLoader.Run())
			return false;

		if (!_imageMatcher.Run())
			return false;

		if (!_resultHandler.Run())
			return false;

		return true;
	}
}

