using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

public class StartGame : MonoBehaviour
{
    public List<GameObject> rewardNumber, rewards, balls;
    List<Vector3> originalPositions, rewardPositions, rewardRotations;
    int numberOfCups;
    public GameObject pointsBoard, spawner;
    // Start is called before the first frame update
    void Start()
    { 
    }
    void Awake()
    {
        originalPositions = new List<Vector3>();
        rewardPositions = new List<Vector3>();
        rewardRotations = new List<Vector3>();
        numberOfCups = transform.childCount;
        for(int i = 0; i < numberOfCups; i++)
        {
            originalPositions.Add(transform.GetChild(i).position);
        }
        for(int i = 0; i < 5; i++)
        {
            rewardPositions.Add(rewards[i].GetComponent<RewardObject>().originalPosition);
            rewardRotations.Add(rewards[i].GetComponent<RewardObject>().originalRotation);
        }
    }

    public void startThisGame()
    {
        gameObject.SetActive(true);
        for (int i = 0; i < numberOfCups; i++)
        {
            transform.GetChild(i).GetComponent<Rigidbody>().velocity = transform.GetChild(i).forward * 0f;
            transform.GetChild(i).rotation = Quaternion.Euler(180f, 0f, 0f);
            transform.GetChild(i).transform.position = originalPositions[i];
            transform.GetChild(i).GetComponent<CupScript>().awardPoint = true;
        }
        for (int i = 0; i < 5; i++)
        {
            rewards[i].GetComponent<Rigidbody>().velocity = rewards[i].transform.forward * 0f;
            rewards[i].transform.rotation = Quaternion.Euler(rewardRotations[i]);
            rewards[i].transform.position = rewardPositions[i];
            rewards[i].GetComponent<RewardObject>().stopGravity();
        }
        rewardNumber[0].GetComponentInChildren<TextMeshProUGUI>().text = ((numberOfCups) / 5).ToString();
        rewardNumber[0].GetComponentInChildren<TextMeshProUGUI>().color = Color.white;
        rewardNumber[1].GetComponentInChildren<TextMeshProUGUI>().text = ((numberOfCups) * 2 / 5).ToString();
        rewardNumber[1].GetComponentInChildren<TextMeshProUGUI>().color = Color.white;
        rewardNumber[2].GetComponentInChildren<TextMeshProUGUI>().text = ((numberOfCups) * 3 / 5).ToString();
        rewardNumber[2].GetComponentInChildren<TextMeshProUGUI>().color = Color.white;
        rewardNumber[3].GetComponentInChildren<TextMeshProUGUI>().text = ((numberOfCups) * 4 / 5).ToString();
        rewardNumber[3].GetComponentInChildren<TextMeshProUGUI>().color = Color.white;
        rewardNumber[4].GetComponentInChildren<TextMeshProUGUI>().text = (numberOfCups).ToString();
        rewardNumber[4].GetComponentInChildren<TextMeshProUGUI>().color = Color.white;
        int nula = 0;
        pointsBoard.GetComponentInChildren<TextMeshProUGUI>().text = nula.ToString();
        Color c = new Color(190f / 255f, 33f / 255f, 33f / 255f);
        pointsBoard.GetComponentInChildren<TextMeshProUGUI>().color = c;
        pointsBoard.GetComponent<PointCounter>().setMaxScore(numberOfCups);
        int numToSpawn = 4;
        if (numberOfCups == 21)
        {
            numToSpawn = 6;
        } 
        else if (numberOfCups == 15)
        {
            numToSpawn = 5;
        }
        spawner.GetComponent<SpawnBalls>().spawnBalls(numToSpawn);
    }

    public void endThisGame()
    {
        gameObject.SetActive(false);
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
