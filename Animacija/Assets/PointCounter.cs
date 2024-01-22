using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

public class PointCounter : MonoBehaviour
{
    public List<GameObject> awardBoards;
    public int maxScore;
    public void setMaxScore(int score)
    {
        maxScore = score;
    }
    public void updateScore()
    {
        int points = int.Parse(GetComponentInChildren<TextMeshProUGUI>().text);
        points++;
        GetComponentInChildren<TextMeshProUGUI>().text = (points).ToString();
        if(points == maxScore)
        {
            transform.GetComponentInChildren<TextMeshProUGUI>().color = Color.green;
        }
        for (int i = 0; i < awardBoards.Count; i++)
        {
            awardBoards[i].GetComponent<PointChecker>().checkPoints(points);
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
