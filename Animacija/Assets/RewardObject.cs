using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RewardObject : MonoBehaviour
{
    public Vector3 originalPosition, originalRotation;
    private void OnCollisionEnter(Collision collision)
    {
        GetComponent<Rigidbody>().useGravity = true;
    }
    // Start is called before the first frame update
    void Start()
    {
        originalPosition = transform.position;
        originalRotation = transform.eulerAngles;
    }

    public void stopGravity()
    {
        GetComponent<Rigidbody>().useGravity = false;
    }

    // Update is called once per frame
    void Update()
    {
        if(originalPosition != transform.position)
        {
            GetComponent<Rigidbody>().useGravity = true;
        }
    }
}
