using UnityEngine;
using LitJson;
using System;
using System.Text;
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
			JsonData chidren = new JsonData();
			node["c"] = chidren;
			root.Add(node);
			AddChildNode(go, ref chidren);
		}

		string msg = root.ToJson();
		msg = msg.Replace("\"c\":}", "\"c\":[]}");
		mClient.SendToServer((int)DTool_CTS.DTool_CTS_UpdateHierarchy, msg);
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
			JsonData chidren = new JsonData();
			node["c"] = chidren;
			jsonRoot.Add(node);
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

	static GameObject FindObject(string path)
	{
		GameObject go = GameObject.Find(path);
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
		GameObject go = FindObject(path);
		if (go == null)
			return;
		int active = 0;
		if (go.activeSelf)
			active = 1;

		JsonData jsonData = new JsonData();
		jsonData["p"] = path;
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
		int active = 0;
		if (!int.TryParse(json["a"].ToString(), out active))
		{
			return;
		}
		GameObject go = FindObject(path);
		if (go == null)
			return;
		go.SetActive(active > 0);
	}

	public void Init()
	{
		mCBMsg[(int)DTool_STC.DTool_STC_ReqObject] = Msg_ReqObject;
		mCBMsg[(int)DTool_STC.DTool_STC_ReqActive] = Msg_ReqActive;
	}

	DToolClient mClient;
	public delegate void cbMsg(JsonData json, DToolClient client);
	public cbMsg[] mCBMsg = new cbMsg[(int)DTool_STC.DTool_STC_Count];
}