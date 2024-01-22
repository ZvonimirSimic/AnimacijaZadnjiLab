using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

public class PointChecker : MonoBehaviour
{
    public void checkPoints(int num)
    {
        if(num >= int.Parse(transform.GetComponentInChildren<TextMeshProUGUI>().text))
        {
            transform.GetComponentInChildren<TextMeshProUGUI>().color = Color.green;
        }
    }   
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
