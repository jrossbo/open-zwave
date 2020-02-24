//-----------------------------------------------------------------------------
//
//	WakeUp.h
//
//	Implementation of the Z-Wave COMMAND_CLASS_WAKE_UP
//
//	Copyright (c) 2010 Mal Lansell <openzwave@lansell.org>
//
//	SOFTWARE NOTICE AND LICENSE
//
//	This file is part of OpenZWave.
//
//	OpenZWave is free software: you can redistribute it and/or modify
//	it under the terms of the GNU Lesser General Public License as published
//	by the Free Software Foundation, either version 3 of the License,
//	or (at your option) any later version.
//
//	OpenZWave is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU Lesser General Public License for more details.
//
//	You should have received a copy of the GNU Lesser General Public License
//	along with OpenZWave.  If not, see <http://www.gnu.org/licenses/>.
//
//-----------------------------------------------------------------------------

#ifndef _WakeUp_H
#define _WakeUp_H

#include <list>
#include "command_classes/CommandClass.h"
#include "Driver.h"
#include "TimerThread.h"

namespace OpenZWave
{

	namespace Internal
	{
		namespace Platform
		{
			class Mutex;
		}
		namespace CC
		{

			/** \brief Implements COMMAND_CLASS_WAKE_UP (0x84), a Z-Wave device command class.
			 * \ingroup CommandClass
			 */
			class WakeUp: public CommandClass, private Timer
			{
				public:
					static CommandClass* Create(uint32 const _homeId, uint8 const _nodeId)
					{
						return new WakeUp(_homeId, _nodeId);
					}
					virtual ~WakeUp();

					static ZW_CommandClasses const StaticGetCommandClassId()
					{
						return ZW_CommandClasses::Wake_Up;
					}
					static string const StaticGetCommandClassName()
					{
						return "COMMAND_CLASS_WAKE_UP";
					}

					void Init();	// Starts the process of requesting node state from a sleeping device.
					void QueueMsg(Driver::MsgQueueItem const& _item);

					/** \brief Send all pending messages followed by a no more information message. */
					void SendPending();

					/** \brief Send a no more information message. */
					void SendNoMoreInfo(uint32 id);

					bool IsAwake() const
					{
						return m_awake;
					}
					void SetAwake(bool _state);
					void SetPollRequired()
					{
						m_pollRequired = true;
					}

					// From CommandClass
					virtual bool RequestState(uint32 const _requestFlags, uint8 const _instance, Driver::MsgQueue const _queue) override;
					virtual bool RequestValue(uint32 const _requestFlags, uint16 const _index, uint8 const _instance, Driver::MsgQueue const _queue) override;
					virtual ZW_CommandClasses const GetCommandClassId() const override
					{
						return StaticGetCommandClassId();
					}
					virtual string const GetCommandClassName() const override
					{
						return StaticGetCommandClassName();
					}
					virtual bool HandleMsg(uint8 const* _data, uint32 const _length, uint32 const _instance = 1) override;
					virtual bool SetValue(Internal::VC::Value const& _value) override;

					virtual uint8 GetMaxVersion() override
					{
						return 2;
					}

					bool supportsMultiInstance() override {
						return false;
					}
				protected:
					virtual void CreateVars(uint8 const _instance) override;

				private:
					WakeUp(uint32 const _homeId, uint8 const _nodeId);

					Internal::Platform::Mutex* m_mutex;			// Serialize access to the pending queue
					list<Driver::MsgQueueItem> m_pendingQueue;		// Messages waiting to be sent when the device wakes up
					bool m_awake;
					bool m_pollRequired;
					uint32 m_interval;
					
			};
		} // namespace CC
	} // namespace Internal
} // namespace OpenZWave

#endif

