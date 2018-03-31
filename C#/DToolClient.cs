using UnityEngine;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Collections;
using System.Collections.Generic;

public enum DTool_STC
{
	DTool_STC_None					= 0,
	DTool_STC_ReqObject				= 1,
	DTool_STC_ReqActive				= 2,

	DTool_STC_Count
}

public enum DTool_CTS
{
	DTool_CTS_None					= 0,
	DTool_CTS_UpdateHierarchy		= 1,
	DTool_CTS_UpdateObject			= 2,
	DTool_CTS_AddLog				= 3,

	DTool_CTS_Count
}

public class DToolClient : MonoBehaviour
{
	void Start()
	{
		InitServer();
	}

	void InitServer()
	{
		IPAddress ipAdr = IPAddress.Parse("127.0.0.1");
		ipep = new IPEndPoint(ipAdr, 5304);

		clientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
		clientSocket.Connect(ipep);

		thread = new Thread(new ThreadStart(GoClient));
		thread.Start();

		logic = new DToolLogic(this);
		logic.Init();
		StartCoroutine(DelayInit());
	}

	IEnumerator DelayInit()
	{
		yield return new WaitForEndOfFrame();

		logic.UpdateHierarchy();
	}

	void GoClient()
	{
		while (true)
		{
			if (clientSocket == null)
				return;
			byte[] receiveData = new byte[1024];
			int recv = clientSocket.Receive(receiveData);
			if (recv == 0)
			{
				Connect();
				continue;
			}

			lock (this)
			{
				mRecvData = receiveData;
				mRecvLen = recv;
			}

			//Debug.LogError(msg);
		}
	}

	void Connect()
	{
		if (clientSocket != null)
		{
			//clientSocket.Shutdown(SocketShutdown.Both);
			clientSocket.Close();
			clientSocket = null;
			return;
		}
		clientSocket.Connect(ipep);
	}

	public void SendToServer(int ctsType, string msg)
	{
		if (clientSocket == null)
			return;

		byte[] intBuff = System.BitConverter.GetBytes(ctsType);
		byte[] concent = Encoding.UTF8.GetBytes(msg);
		byte[] tmp = new byte[intBuff.Length + concent.Length];
		System.Buffer.BlockCopy(intBuff, 0, tmp, 0, intBuff.Length);
		System.Buffer.BlockCopy(concent, 0, tmp, intBuff.Length, concent.Length);
		try
		{
			int count = clientSocket.Send(tmp, tmp.Length, SocketFlags.None);
		}
		catch (SocketException e)
		{
			Debug.LogError(e.Message);
		}
	}

	void OnEnable()
	{
		Application.logMessageReceived += HandleLog;
	}

	void OnDisable()
	{
		clientSocket.Shutdown(SocketShutdown.Both);
		clientSocket.Close();

		if (thread != null)
		{
			thread.Interrupt();
			thread.Abort();
		}
	}

	void Update()
	{
		lock (this)
		{
			if (mRecvData != null && mRecvLen > 0)
			{
				int stc = System.BitConverter.ToInt32(mRecvData, 0);
				string msg = Encoding.UTF8.GetString(mRecvData, 4, mRecvLen - 4);

				logic.OnMsg((DTool_STC)stc, msg);

				mRecvData = null;
				mRecvLen = 0;
			}
		}
	}

	void HandleLog(string message, string stack, LogType type)
	{
		logic.AddLog(message, stack, type);
	}

	Thread thread;
	Socket clientSocket = null;
	IPEndPoint ipep;
	public DToolLogic logic = null;
	byte[] mRecvData = null;
	int mRecvLen = 0;
}