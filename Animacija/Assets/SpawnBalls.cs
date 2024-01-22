using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SpawnBalls : MonoBehaviour
{
    List<GameObject> balls;
    public GameObject ball;
    private int toSpawn = 0;
    private float lastTime;
    // Start is called before the first frame update
    void Start()
    {
        balls = new List<GameObject>();
        lastTime = Time.time;
    }

    public void spawnBalls(int num)
    {
        if (balls.Count == 0) {
            toSpawn = num;
        }
        else
        {
            for(int i = balls.Count-1; i >= 0; i--)
            {
                Destroy(balls[i].gameObject);
            }
            balls.Clear();
            toSpawn = num;
        }
    }

    // Update is called once per frame
    void Update()
    {
        if(toSpawn > 0)
        {
            if(Time.time - lastTime > 1f)
            {
                Vector3 position = transform.position;
                position.x += (float)Random.Range(-8,8) / 100;
                position.z += (float)Random.Range(-8, 8) / 100;
                GameObject newBall = Instantiate(ball, position, Quaternion.identity);
                balls.Add(newBall);
                toSpawn--;
                lastTime = Time.time;
            }
        }
    }
}
