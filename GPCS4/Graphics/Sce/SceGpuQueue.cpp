#include "SceGpuQueue.h"

#include "Gnm/GnmCommandBufferDispatch.h"
#include "Gnm/GnmCommandBufferDraw.h"
#include "Gnm/GnmCommandBufferDummy.h"
#include "Gnm/GnmCommandProcessor.h"
#include "Violet/VltDevice.h"
#include "Violet/VltCmdList.h"

LOG_CHANNEL(Graphic.Sce.SceGpuQueue);

namespace sce
{
	using namespace Gnm;
	using namespace vlt;

	SceGpuQueue::SceGpuQueue(
		vlt::VltDevice* device,
		SceQueueType    type) :
		m_device(device)
	{
		createQueue(type);
	}

	SceGpuQueue::~SceGpuQueue()
	{
	}

	void SceGpuQueue::record(const SceGpuCommand& cmd)
	{
		m_cp->processCommandBuffer(cmd.buffer, cmd.size);
	}

	void SceGpuQueue::submit(const SceGpuSubmission& submission)
	{
		m_device->submitCommandList(
			m_cmd->finalize(),
			submission.wait,
			submission.wake);
	}

	void SceGpuQueue::synchronize()
	{
		m_device->syncSubmission();
	}

	void SceGpuQueue::createQueue(SceQueueType type)
	{
		m_cp = std::make_unique<GnmCommandProcessor>();

		if (type == SceQueueType::Graphics)
		{
			m_cmd = std::make_unique<GnmCommandBufferDraw>(m_device);
		}
		else
		{
			m_cmd = std::make_unique<GnmCommandBufferDispatch>(m_device);
		}

#ifdef GPCS4_NO_GRAPHICS
		m_cmd = std::make_unique<GnmCommandBufferDummy>(m_device);
#endif

		m_cp->attachCommandBuffer(m_cmd.get());
	}

}  // namespace sce