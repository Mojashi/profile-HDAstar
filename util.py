from typing import List
import json
import numpy as np
import pandas as pd

class Result:
    processNum: int
    height: int
    width: int 
    obstacleRatio: int
    PQSizePercentile: List[int]

def calcResults(logNames: List[str], fold: bool):
    datas = []
    for logName in logNames:
        data = json.load(open(logName, "r"))
        print(data['conditions'])
        datas.append(data['results'])

    results = []

    unfolded = lambda row: [row['processNum'],row['height'],row['width'],row['obstacleRatio'],row['neighborsNum'],row['iterNum'],row['PQSizePercentile'][99]]
    folded = lambda row: [row['processNum'],max(row['height'],row['width']),min(row['height'],row['width']),row['obstacleRatio'],row['neighborsNum'],row['iterNum'],row['PQSizePercentile'][99]]
    for data in datas:
        results.append(pd.DataFrame(data=list(
            map(folded if fold else unfolded,
            np.array(data))
        ), columns=['processNum','height','width','obstacleRatio','neighborsNum','iterNum', 'MAXPQSize']))

    results = pd.concat(results, axis=0)
    return results
