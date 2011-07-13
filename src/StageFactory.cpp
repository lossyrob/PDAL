/******************************************************************************
* Copyright (c) 2011, Michael P. Gerlek (mpg@flaxen.com)
*
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following
* conditions are met:
*
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in
*       the documentation and/or other materials provided
*       with the distribution.
*     * Neither the name of Hobu, Inc. or Flaxen Geo Consulting nor the
*       names of its contributors may be used to endorse or promote
*       products derived from this software without specific prior
*       written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
* OF SUCH DAMAGE.
****************************************************************************/

#include <pdal/StageFactory.hpp>

#include <pdal/Filter.hpp>
#include <pdal/MultiFilter.hpp>
#include <pdal/Reader.hpp>
#include <pdal/Writer.hpp>

#include <pdal/drivers/las/Reader.hpp>
#include <pdal/drivers/las/Writer.hpp>
#include <pdal/drivers/liblas/Reader.hpp>
#include <pdal/drivers/liblas/Writer.hpp>

#include <boost/shared_ptr.hpp>


namespace pdal
{
    

static Reader* create_drivers_las_reader(const Options& options)       { return new pdal::drivers::las::LasReader("../../test/data/1.0_0.las"); }
static Reader* create_drivers_liblas_reader(const Options& options)    { return new pdal::drivers::liblas::LiblasReader("../../test/data/1.0_0.las"); }


StageFactory::StageFactory()
{
    registerKnownStages();

    return;
}


boost::shared_ptr<Reader> StageFactory::createReader(const std::string& type, const Options& options)
{
    readerCreatorFunction* f = getReaderCreator(type);
    Reader* stage = f(options);
    boost::shared_ptr<Reader> ptr(stage);
    return ptr;
}


boost::shared_ptr<Filter> StageFactory::createFilter(const std::string& type, boost::uint32_t prevStage, const Options& options)
{
    filterCreatorFunction* f = getFilterCreator(type);
    Filter* stage = f(prevStage, options);
    boost::shared_ptr<Filter> ptr(stage);
    return ptr;
}


boost::shared_ptr<MultiFilter> StageFactory::createMultiFilter(const std::string& type, const std::vector<boost::uint32_t>& prevStages, const Options& options)
{
    multifilterCreatorFunction* f = getMultiFilterCreator(type);
    MultiFilter* stage = f(prevStages, options);
    boost::shared_ptr<MultiFilter> ptr(stage);
    return ptr;
}


boost::shared_ptr<Writer> StageFactory::createWriter(const std::string& type, boost::uint32_t prevStage, const Options& options)
{
    writerCreatorFunction* f = getWriterCreator(type);
    Writer* stage = f(prevStage, options);
    boost::shared_ptr<Writer> ptr(stage);
    return ptr;
}


StageFactory::readerCreatorFunction* StageFactory::getReaderCreator(const std::string& type)
{
    std::map<std::string, readerCreatorFunction*>::const_iterator iter = m_readerCreators.find(type);
    if (iter == m_readerCreators.end()) throw;
    readerCreatorFunction* f = iter->second;
    return f;
}


StageFactory::filterCreatorFunction* StageFactory::getFilterCreator(const std::string& type)
{
    std::map<std::string, filterCreatorFunction*>::const_iterator iter = m_filterCreators.find(type);
    if (iter == m_filterCreators.end()) throw;
    filterCreatorFunction* f = iter->second;
    return f;
}


StageFactory::multifilterCreatorFunction* StageFactory::getMultiFilterCreator(const std::string& type)
{
    std::map<std::string, multifilterCreatorFunction*>::const_iterator iter = m_multifilterCreators.find(type);
    if (iter == m_multifilterCreators.end()) throw;
    multifilterCreatorFunction* f = iter->second;
    return f;
}


StageFactory::writerCreatorFunction* StageFactory::getWriterCreator(const std::string& type)
{
    std::map<std::string, writerCreatorFunction*>::const_iterator iter = m_writerCreators.find(type);
    if (iter == m_writerCreators.end()) throw;
    writerCreatorFunction* f = iter->second;
    return f;
}


void StageFactory::registerReader(const std::string& type, readerCreatorFunction* f)
{
    std::pair<std::string, readerCreatorFunction*> p(type, f);
    m_readerCreators.insert(p);
}


void StageFactory::registerFilter(const std::string& type, filterCreatorFunction* f)
{
    std::pair<std::string, filterCreatorFunction*> p(type, f);
    m_filterCreators.insert(p);
}


void StageFactory::registerMultiFilter(const std::string& type, multifilterCreatorFunction* f)
{
    std::pair<std::string, multifilterCreatorFunction*> p(type, f);
    m_multifilterCreators.insert(p);
}


void StageFactory::registerWriter(const std::string& type, writerCreatorFunction* f)
{
    std::pair<std::string, writerCreatorFunction*> p(type, f);
    m_writerCreators.insert(p);
}


void StageFactory::registerKnownStages()
{
    registerReader("drivers.las.reader", create_drivers_las_reader);
    registerReader("drivers.liblas.reader", create_drivers_liblas_reader);
}


} // namespace pdal
