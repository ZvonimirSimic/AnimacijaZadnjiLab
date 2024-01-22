using BNG;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CloseApplication : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (ControllerBinding.AButtonDown.GetDown() || ControllerBinding.BButtonDown.GetDown() ||
            ControllerBinding.XButtonDown.GetDown() || ControllerBinding.YButtonDown.GetDown())
        {
            Application.Quit();
        }
    }
}
