#include "rtmppush_private.h"
#include "rtmppush_stream.h"
#include "base/publisher/application.h"
#include "base/publisher/stream.h"

#include <regex>

std::shared_ptr<RtmpPushStream> RtmpPushStream::Create(const std::shared_ptr<pub::Application> application,
											 const info::Stream &info)
{
	auto stream = std::make_shared<RtmpPushStream>(application, info);
	if(!stream->Start())
	{
		return nullptr;
	}

	if(!stream->CreateStreamWorker(2))
	{
		return nullptr;
	}

	return stream;
}

RtmpPushStream::RtmpPushStream(const std::shared_ptr<pub::Application> application,
					 const info::Stream &info)
		: Stream(application, info),
		_parsed_flag(false)
{
	_writer = nullptr;
}

RtmpPushStream::~RtmpPushStream()
{
	logtd("RtmpPushStream(%s/%s) has been terminated finally", 
		GetApplicationName() , GetName().CStr());
}

bool RtmpPushStream::Start()
{
	logtd("RtmpPushStream(%ld) has been started", GetId());

	return Stream::Start();
}

bool RtmpPushStream::Stop()
{
	logtd("RtmpPushStream(%u) has been stopped", GetId());

	return Stream::Stop();
}

void RtmpPushStream::SendVideoFrame(const std::shared_ptr<MediaPacket> &media_packet)
{	
	auto stream_packet = std::make_any<std::shared_ptr<MediaPacket>>(media_packet);

	BroadcastPacket(stream_packet);
}

void RtmpPushStream::SendAudioFrame(const std::shared_ptr<MediaPacket> &media_packet)
{
	auto stream_packet = std::make_any<std::shared_ptr<MediaPacket>>(media_packet);

	BroadcastPacket(stream_packet);
}

bool RtmpPushStream::DeleteSession(uint32_t session_id)
{
	return RemoveSession(session_id);
}

std::shared_ptr<RtmpPushSession> RtmpPushStream::CreateSession()
{
	auto session = RtmpPushSession::Create(GetApplication(), GetSharedPtrAs<pub::Stream>(), this->IssueUniqueSessionId());
	if(session == nullptr)
	{
		logte("Internal Error : Cannot create session");
		return nullptr;
	}

	logtd("created session");

	AddSession(session);

	return session;
}

void RtmpPushStream::SetParsed(bool flag)
{
	_parsed_flag = flag;
}
bool RtmpPushStream::IsParsed()
{
	return _parsed_flag;
}