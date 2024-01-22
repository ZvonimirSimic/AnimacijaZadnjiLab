using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

public class CupScript : MonoBehaviour
{
    public GameObject pointBoard;
    public bool awardPoint = true;
    private float startingY;
    // Start is called before the first frame update
    void Start()
    {
        startingY = transform.position.y;
    }

    public void givePoint()
    {
        pointBoard.GetComponentInChildren<PointCounter>().updateScore();
    }
    // Update is called once per frame
    void Update()
    {
        if(startingY - transform.position.y >= 0.15 && awardPoint)
        {
            givePoint();
            awardPoint = false;
        }
    }
}
