using UnityEngine;
using LitJson;
using System;
using System.Text;
using System.Collections;
using System.Collections.Generic;

public class DToolLogic
{
	public DToolLogic(DToolClient client)
	{
		mClient = client;
	}

	public void UpdateHierarchy()
	{
		JsonData root = new JsonData();
		GameObject[] gos = GameObject.FindObjectsOfType<GameObject>();//UnityEngine.Profiling.Profiler;
		for (int i = 0; i < gos.Length; ++i)
		{
			GameObject go = gos[i];
			if (go.transform.parent != null)
				continue;
			string nameInfo = "";
			StringBuilder sb = new StringBuilder(nameInfo);
			int active = 0;
			if (go.activeSelf)
				active = 1;
			sb.Append(go.name).Append ("+").Append(active);
			JsonData node = new JsonData();
			node["n"] = go.name;
			node["a"] = active;
			node["i"] = go.GetInstanceID();
			JsonData chidren = new JsonData();
			node["c"] = chidren;
			root.Add(node);

			mClient.mSceneGameObjects[go.GetInstanceID()] = go;

			AddChildNode(go, ref chidren);
		}

		string msg = root.ToJson();
		msg = msg.Replace("\"c\":}", "\"c\":[]}");

		mClient.SendToServer((int)DTool_CTS.DTool_CTS_UpdateHierarchy, msg);

		return;

		int maxLen = int.MaxValue;
		int groupCount = Mathf.CeilToInt(((float)msg.Length + 6.0f) / (float)maxLen);
		string[] msgGroups = new string[groupCount];

		mClient.SetBigMsgLock(true);
		for (int i = 0; i < msgGroups.Length; ++i)
		{
			msgGroups[i] = "";
			if (i == 0)
				msgGroups[i] += "BIG";
			
			if (i == msgGroups.Length - 1)
			{
				msgGroups[i] += msg.Substring(i * maxLen);
				msgGroups[i] += "BIG";
			}
			else
			{
				msgGroups[i] += msg.Substring(i * maxLen, maxLen);
			}
			//mClient.SendToServer((int)DTool_CTS.DTool_CTS_UpdateHierarchy, msgGroups[i]);
			mClient.StartCoroutine(SendHierarchyInfo(msgGroups[i], 0.2f * (float)i, i == msgGroups.Length - 1));
		}


		//mClient.SendToServer((int)DTool_CTS.DTool_CTS_UpdateHierarchy, msg);
	}

	IEnumerator SendHierarchyInfo(string msg, float delay, bool unlock)
	{
		yield return new WaitForSeconds(delay);
		mClient.SendToServer((int)DTool_CTS.DTool_CTS_UpdateHierarchy, msg, true);
		if (unlock)
		{
			mClient.SetBigMsgLock(false);
		}
	}

	void AddChildNode(GameObject rootGO, ref JsonData jsonRoot)
	{
		List<GameObject> children = new List<GameObject>();
		for (int i = 0; i < rootGO.transform.childCount; ++i)
		{
			Transform tChild = rootGO.transform.GetChild(i);
			GameObject go = tChild.gameObject;

			string nameInfo = "";
			StringBuilder sb = new StringBuilder(nameInfo);
			int active = 0;
			if (go.activeSelf)
				active = 1;
			sb.Append(go.name).Append ("+").Append(active);
			JsonData node = new JsonData();
			node["n"] = go.name;
			node["a"] = active;
			node["i"] = go.GetInstanceID();
			JsonData chidren = new JsonData();
			node["c"] = chidren;
			jsonRoot.Add(node);

			mClient.mSceneGameObjects[go.GetInstanceID()] = go;

			AddChildNode(go, ref chidren);
		}
	}

	public void AddLog(string message, string stack, LogType type)
	{
		JsonData node = new JsonData();
		node["m"] = message;
		node["s"] = stack;
		node["t"] = (int)type;
		string msg = node.ToJson();
		mClient.SendToServer((int)DTool_CTS.DTool_CTS_AddLog, msg);
	}

	public void OnMsg(DTool_STC stcType, string msg)
	{
		cbMsg cb = mCBMsg[(int)stcType];
		if (cb != null)
		{
			JsonData json = JsonMapper.ToObject<JsonData>(msg);
			cb(json, mClient);
		}
	}

	static GameObject FindObject(string path, DToolClient client, int instanceID)
	{
		GameObject go = null;
		bool isFind = client.mSceneGameObjects.TryGetValue(instanceID, out go);
		if (isFind)
		{
			return go;
		}

		go = GameObject.Find(path);
		if (go == null)
		{
			int index = path.IndexOf('/');
			if (index <= -1)
				return null;
			string headPath = path.Substring(0, index);
			GameObject root = GameObject.Find(headPath);
			if (root == null)
				return null;
			path = path.Substring(index + 1);
			Transform t = root.transform.Find(path);
			if (t == null)
				return null;
			go = t.gameObject;
		}
		return go;
	}



	////////////////////////////////////////////////////////////RegisterCallback////////////////////////////////////////////////////////////////////////////////////

	static void Msg_ReqObject(JsonData json, DToolClient client)
	{
		string path = json["p"].ToString();
		int instanceID = json["i"].AsInt;
		GameObject go = FindObject(path, client, instanceID);
		if (go == null)
			return;
		int active = 0;
		if (go.activeSelf)
			active = 1;

		JsonData jsonData = new JsonData();
		jsonData["p"] = path;
		jsonData["i"] = go.GetInstanceID();
		jsonData["a"] = active;
		jsonData["px"] = go.transform.position.x;
		jsonData["py"] = go.transform.position.y;
		jsonData["pz"] = go.transform.position.z;
		jsonData["rx"] = go.transform.eulerAngles.x;
		jsonData["ry"] = go.transform.eulerAngles.y;
		jsonData["rz"] = go.transform.eulerAngles.z;
		jsonData["sx"] = go.transform.localScale.x;
		jsonData["sy"] = go.transform.localScale.y;
		jsonData["sz"] = go.transform.localScale.z;
		client.SendToServer((int)DTool_CTS.DTool_CTS_UpdateObject, jsonData.ToJson());
	}

	static void Msg_ReqActive(JsonData json, DToolClient client)
	{
		string path = json["p"].ToString();
		int instanceID = json["i"].AsInt;
		int active = 0;
		if (!int.TryParse(json["a"].ToString(), out active))
		{
			return;
		}
		GameObject go = FindObject(path, client, instanceID);
		if (go == null)
			return;
		go.SetActive(active > 0);
	}

	public enum MemoryObjType
	{
		MemoryObjType_RenderTexture,
		MemoryObjType_Texture2D,
		MemoryObjType_CubeMap,
		MemoryObjType_Mesh,
		MemoryObjType_AnimationClip,
	}

	static void Msg_ReqObjMemory(JsonData json, DToolClient client)
	{
		int objType = 0;
		if (!int.TryParse(json["t"].ToString(), out objType))
			return;
		MemoryObjType mot = (MemoryObjType)objType;

		Type tp = typeof(UnityEngine.Object);
		switch (mot)
		{
		case MemoryObjType.MemoryObjType_RenderTexture:
			tp = typeof(RenderTexture);
			break;
		case MemoryObjType.MemoryObjType_Texture2D:
			tp = typeof(Texture2D);
			break;
		case MemoryObjType.MemoryObjType_CubeMap:
			tp = typeof(Cubemap);
			break;
		case MemoryObjType.MemoryObjType_Mesh:
			tp = typeof(Mesh);
			break;
		case MemoryObjType.MemoryObjType_AnimationClip:
			tp = typeof(AnimationClip);
			break;
		}

		JsonData rootJson = new JsonData();
		JsonData arrayJson = new JsonData();
		long totalSize = 0;
		UnityEngine.Object[] objs = Resources.FindObjectsOfTypeAll(tp);
		for (int i = 0; i < objs.Length; ++i)
		{
			JsonData jsonData = new JsonData();
			jsonData["t"] = objType;

			UnityEngine.Object obj = objs[i];
			switch (mot)
			{
			case MemoryObjType.MemoryObjType_RenderTexture:
				{
					RenderTexture rt = obj as RenderTexture;
					jsonData["n"] = rt.name;
					jsonData["w"] = rt.width;
					jsonData["h"] = rt.height;
					jsonData["d"] = rt.depth;
					jsonData["f"] = (int)rt.format;
				}
				break;
			case MemoryObjType.MemoryObjType_Texture2D:
				{
					Texture2D tex = obj as Texture2D;
					jsonData["n"] = tex.name;
					jsonData["w"] = tex.width;
					jsonData["h"] = tex.height;
					jsonData["m"] = tex.mipmapCount;
					jsonData["f"] = (int)tex.format;
				}
				break;
			case MemoryObjType.MemoryObjType_CubeMap:
				{
					Cubemap cubmap = obj as Cubemap;
					jsonData["n"] = cubmap.name;
					jsonData["w"] = cubmap.width;
					jsonData["h"] = cubmap.height;
					jsonData["f"] = (int)cubmap.format;
					jsonData["m"] = cubmap.mipmapCount;
				}
				break;
			case MemoryObjType.MemoryObjType_Mesh:
				{
					Mesh mesh = obj as Mesh;
					jsonData["n"] = mesh.name;
					jsonData["r"] = mesh.isReadable;
					jsonData["v"] = mesh.vertexCount;
					jsonData["smc"] = mesh.subMeshCount;
					JsonData smdata = new JsonData();
					jsonData["sm"] = smdata;
					for (int s = 0; s < mesh.subMeshCount; ++s)
					{
						smdata.Add((int)mesh.GetIndexCount(s));
					}
				}
				break;
			case MemoryObjType.MemoryObjType_AnimationClip:
				{
					AnimationClip clip = obj as AnimationClip;
					jsonData["n"] = clip.name;
					jsonData["l"] = clip.length;
					jsonData["il"] = clip.isLooping ? 1 : 0;
				}
				break;
			}
			long msz = UnityEngine.Profiling.Profiler.GetRuntimeMemorySizeLong(obj);
			jsonData["sz"] = msz;
			totalSize += msz;

			arrayJson.Add(jsonData);
		}
		if (arrayJson.IsArray)
		{
			rootJson["a"] = arrayJson;
		}
		rootJson["s"] = totalSize;
		client.SendToServer((int)DTool_CTS.DTool_CTS_ObjMemory, rootJson.ToJson());
	}

	static string ToCommasNumber(long number)
	{
		string szNum = number.ToString();

		int sig = 0;
		for (int i = 0; i < szNum.Length; ++i)
		{
			++sig;
		}
		int b = sig % 3;
		b = 3 - b;

		char[] szCommas = new char[64];
		for (int i = 0; i < 64; ++i)
			szCommas[i] = '\0';

		for (int i = 0, index = 0; i < sig; ++index)
		{
			if (b == 3)
			{
				b = 0;
				if (i != 0)
				{
					szCommas[index] = ',';
					continue;
				}
			}
			szCommas[index] = szNum[i];
			++b;
			++i;
		}

		string rst = new string(szCommas);
		return rst;
	}

	static void Msg_ReqMemory(JsonData json, DToolClient client)
	{
		JsonData jsonRoot = new JsonData ();
		jsonRoot["trm"] = ToCommasNumber(UnityEngine.Profiling.Profiler.GetTotalReservedMemoryLong()); // 总堆内存;
		jsonRoot["tam"] = ToCommasNumber(UnityEngine.Profiling.Profiler.GetTotalAllocatedMemoryLong()); // 已占用内存;
		jsonRoot["turm"] = ToCommasNumber(UnityEngine.Profiling.Profiler.GetTotalUnusedReservedMemoryLong()); // 空闲中内存;
		jsonRoot["mhs"] = ToCommasNumber(UnityEngine.Profiling.Profiler.GetMonoHeapSizeLong()); // 总Mono堆内存;
		jsonRoot["mus"] = ToCommasNumber(UnityEngine.Profiling.Profiler.GetMonoUsedSizeLong()); // 已占用Mono堆内存;

		client.SendToServer((int)DTool_CTS.DTool_CTS_Memory, jsonRoot.ToJson());
	}

	static void Msg_ReqUpdateHierarchy(JsonData json, DToolClient client)
	{
		client.logic.UpdateHierarchy();
	}

	static void Msg_ReqGraphicBase(JsonData json, DToolClient client)
	{
		long triNum = 0;
		long vertNum = 0;
		GameObject[] objs = GameObject.FindObjectsOfType<GameObject>();
		for (int i = 0; i < objs.Length; ++i)
		{
			GameObject obj = objs[i];
			MeshFilter[] filters = obj.GetComponentsInChildren<MeshFilter>();
			for (int j = 0; j < filters.Length; ++j)
			{
				MeshFilter filter = filters[j];
				if (!filter.sharedMesh.isReadable)
					continue;
				triNum += filter.sharedMesh.triangles.Length / 3;
				vertNum += filter.sharedMesh.vertexCount;
			}
		}

		JsonData jsonRoot = new JsonData ();
		jsonRoot["fnum"] = ToCommasNumber(triNum);
		jsonRoot["vnum"] = ToCommasNumber(vertNum);
		client.SendToServer((int)DTool_CTS.DTool_CTS_GraphicBase, jsonRoot.ToJson());
	}

	public void Init()
	{
		mCBMsg[(int)DTool_STC.DTool_STC_ReqObject] = Msg_ReqObject;
		mCBMsg[(int)DTool_STC.DTool_STC_ReqActive] = Msg_ReqActive;
		mCBMsg[(int)DTool_STC.DTool_STC_ReqObjMemory] = Msg_ReqObjMemory;
		mCBMsg[(int)DTool_STC.DTool_STC_ReqMemory] = Msg_ReqMemory;
		mCBMsg[(int)DTool_STC.DTool_STC_ReqUpdateHierarchy] = Msg_ReqUpdateHierarchy;
		mCBMsg[(int)DTool_STC.DTool_STC_ReqGraphicBase] = Msg_ReqGraphicBase;
	}

	DToolClient mClient;
	public delegate void cbMsg(JsonData json, DToolClient client);
	public cbMsg[] mCBMsg = new cbMsg[(int)DTool_STC.DTool_STC_Count];
}