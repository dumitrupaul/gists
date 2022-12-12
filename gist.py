#!/usr/bin/env python3
from operator import ne
import numpy as np
from collections import Counter, defaultdict
from igraph import *
from bitstring import *
from queue import PriorityQueue
import networkx as nx
import re as re
import math

count = 0


def flashOnPosition(matrix, lineIdx, colIdx):
    matrix[lineIdx][colIdx] = 0
    if lineIdx != 0:
        matrix[lineIdx - 1][colIdx] += 0 if matrix[lineIdx - 1][colIdx] == 0 else 1
        if colIdx != 0:
            matrix[lineIdx - 1][colIdx - 1] += (
                0 if matrix[lineIdx - 1][colIdx - 1] == 0 else 1
            )
        if colIdx != len(matrix[lineIdx]) - 1:
            matrix[lineIdx - 1][colIdx + 1] += (
                0 if matrix[lineIdx - 1][colIdx + 1] == 0 else 1
            )

    if colIdx != 0:
        matrix[lineIdx][colIdx - 1] += 0 if matrix[lineIdx][colIdx - 1] == 0 else 1

    if lineIdx != len(matrix) - 1:
        matrix[lineIdx + 1][colIdx] += 0 if matrix[lineIdx + 1][colIdx] == 0 else 1
        if colIdx != len(matrix[lineIdx]) - 1:
            matrix[lineIdx + 1][colIdx + 1] += (
                0 if matrix[lineIdx + 1][colIdx + 1] == 0 else 1
            )
        if colIdx != 0:
            matrix[lineIdx + 1][colIdx - 1] += (
                0 if matrix[lineIdx + 1][colIdx - 1] == 0 else 1
            )

    if colIdx != len(matrix[lineIdx]) - 1:
        matrix[lineIdx][colIdx + 1] += 0 if matrix[lineIdx][colIdx + 1] == 0 else 1

    return matrix


def checkFlash(matrix):
    for i in range(len(matrix)):
        for j in range(len(matrix[i])):
            if matrix[i][j] > 9:
                global count
                count += 1
                matrix = flashOnPosition(matrix, i, j)
                return [matrix, True]
    return [matrix, False]


def flashRecursively(matrix):
    result = checkFlash(matrix)
    matrix = result[0]
    while result[1]:
        result = checkFlash(matrix)
        matrix = result[0]
    return matrix


def doOneStep(matrix):
    matrix = np.array(matrix)
    matrix = matrix + 1

    return flashRecursively(matrix)


paths = []


def lookForDuplicates(path):
    for p in path:
        numberOfDuplicates = 0
        if p.islower():
            for p2 in path:
                if p == p2:
                    numberOfDuplicates += 1
            if numberOfDuplicates >= 2:
                return True
    return False


def getAllPaths(graph, start, end, path=[], foundPair=False):
    path.append(start)
    foundPair = lookForDuplicates(path)
    if start == end:
        paths.append(path.copy())
    else:
        for i in graph[start]:
            if i == "start":
                continue
            if foundPair and i.islower() and i in path:
                continue
            getAllPaths(graph, i, end, path, foundPair)
    path.pop()


def mergeLine(mergee, toMerge):
    for i in range(len(mergee)):
        mergee[i] = True if toMerge[i] else mergee[i]
    return mergee


def foldOn(paper, point):
    if point[0] == "y":
        k = 1
        for i in range(point[1] + 1, len(paper)):
            paper[point[1] - k] = mergeLine(paper[point[1] - k], paper[i])

            # print(point[1] - k, paper[point[1] - k])
            k += 1
        for i in range(point[1], len(paper)):
            paper.pop()
    else:
        for i in range(len(paper)):
            lineToCopyOver = paper[i][point[1] + 1 : len(paper[i])]
            lineToCopyOver.reverse()
            # print("linetocopyover: ", lineToCopyOver)
            paper[i] = mergeLine(paper[i][0 : point[1]], lineToCopyOver)
            # print("paper[i]: ", paper[i])
    return paper


def growPolymer(polymer, template):
    newPolymer = []
    savedLastCharIter = -1
    for i in range(len(polymer) - 1):
        pair = polymer[i] + polymer[i + 1]
        value = template.get(pair)
        if value is not None:
            newPolymer.append(
                ("" if savedLastCharIter == i else polymer[i]) + value + polymer[i + 1]
            )
            savedLastCharIter = i + 1
        else:
            newPolymer.append(polymer[i])
    return "".join(newPolymer)


def getOccurences(polymer):
    occurences = {}
    for p in polymer:
        if p in occurences:
            occurences[p] += 1
        else:
            occurences[p] = 0
    return occurences


def growPolymerPart2(polymer, template, steps):
    counter = Counter(["".join(i) for i in zip(polymer, polymer[1:])])

    for _ in range(steps):
        for key, value in counter.copy().items():
            counter[key] -= value
            counter[key[0] + template[key]] += value
            counter[template[key] + key[1]] += value
        print(counter)

    occurences = Counter([polymer[0], polymer[-1]])
    for key, value in counter.items():
        occurences[key[0]] += value
        occurences[key[1]] += value
    result = occurences.most_common()
    print(result)
    return (result[0][1] - result[-1][1]) // 2


versionSum = 0


def unpack(packet, isCounted=False):

    version, type, packet = unpackHeader(packet)
    typeId = ""
    global versionSum
    versionSum += version
    if type == 4:
        _, packet = unpackType4Packet(packet)
    else:
        typeId = packet.read(1).bin
        packet = packet[1:]
        if typeId == "0":
            subpacketsLength = packet.read(15).uint
            packet = packet[15:]
            firstPart = packet[:subpacketsLength]
            unpack(firstPart)
            packet = packet[subpacketsLength:]
        else:
            subpackets = packet.read(11).uint
            packet = packet[11:]
            for i in range(subpackets):
                packet = unpack(packet, True)

    if not isCounted and len(packet) != 0 and packet.uint != 0:
        return unpack(packet)
    else:
        return packet


def unpackType4Packet(packet):
    number = packet.read(5)
    finalNumber = number.bin[1:]
    firstBit = number.bin[0]
    packet = packet[5:]
    while firstBit == "1":
        number = packet.read(5)
        finalNumber += number.bin[1:]
        firstBit = number.bin[0]
        packet = packet[5:]
    return finalNumber, packet


def unpackHeader(packet):
    version, type = packet.readlist("uint:3, uint:3")
    return version, type, packet[6:]


def getPath(matrix, m, n):
    # Instead of following line, we can use int tc[m+1][n+1] or
    # dynamically allocate memoery to save space. The following
    # line is used to keep te program simple and make it working
    # on all compilers.
    tc = [[0 for x in range(len(matrix))] for x in range(len(matrix))]

    tc[0][0] = matrix[0][0]

    # Initialize first column of total cost(tc) array
    for i in range(1, m + 1):
        tc[i][0] = tc[i - 1][0] + matrix[i][0]

    # Initialize first row of tc array
    for j in range(1, n + 1):
        tc[0][j] = tc[0][j - 1] + matrix[0][j]

    # Construct rest of the tc array
    for i in range(1, m + 1):
        for j in range(1, n + 1):
            tc[i][j] = min(tc[i - 1][j], tc[i][j - 1]) + matrix[i][j]

    return tc[m][n]


def isSafe(matrix, x, y):
    return x >= 0 and x < len(matrix) and y >= 0 and y < len(matrix)


def getPathDijkstra(matrix, m, n):
    dp = [[sys.maxsize for i in range(len(matrix))] for j in range(len(matrix))]
    visited = [[False for i in range(len(matrix))] for j in range(len(matrix))]
    dx = [1, -1, 0, 0]
    dy = [0, 0, 1, -1]

    dp[0][0] = matrix[0][0]
    pq = PriorityQueue()
    pq.put((0, 0, matrix[0][0]))

    while not pq.empty():
        (x, y, _) = pq.get()
        if visited[x][y]:
            continue
        visited[x][y] = True
        for i in range(4):
            next_x = x + dx[i]
            next_y = y + dy[i]
            if isSafe(matrix, next_x, next_y) and not visited[next_x][next_y]:
                dp[next_x][next_y] = min(
                    dp[next_x][next_y], dp[x][y] + matrix[next_x][next_y]
                )
                pq.put((next_x, next_y, dp[next_x][next_y]))

    return dp[m][n]


def addSnailfish(x, y):
    return "[" + x + "," + y + "]"


def explodeSnailfish(ex):
    depth = 0
    newEx = []
    exploded = False
    for i, x in enumerate(ex):
        if depth == 5:
            exploded = True
            for j in range(i - 1, 0, -1):
                if ex[j].isdigit():
                    ex[j] = str(int(ex[j]) + int(ex[i]))
                    break
            for j in range(i + 3, len(ex)):
                if ex[j].isdigit():
                    ex[j] = str(int(ex[j]) + int(ex[i + 2]))
                    break
            newEx = ex[: i - 1] + ["0"] + ex[i + 4 :]
            break
        if x == "[":
            depth += 1
        elif x == "]":
            depth -= 1
    return exploded, newEx if exploded else ex


def splitSnailfish(ex):
    newEx = []
    splitted = False
    for i, x in enumerate(ex):
        if x.isnumeric() and int(x) >= 10:
            splitted = True
            inserted = [
                "[",
                str(math.floor(int(x) / 2)),
                ",",
                str(math.ceil(int(x) / 2)),
                "]",
            ]
            newEx = ex[:i] + inserted + ex[i + 1 :]
            break
    return splitted, newEx if splitted else ex


def reduceSnailfish(ex):
    for i in range(1, len(ex)):
        ex[i] = list(addSnailfish(ex[i - 1], ex[i]))
        keepTrying = True
        while keepTrying:
            keepTrying, ex[i] = explodeSnailfish(ex[i])
            if keepTrying:
                continue
            else:
                keepTrying, ex[i] = splitSnailfish(ex[i])
        ex[i] = "".join(x for x in ex[i])
    return list(ex[len(ex) - 1])


def magnitude(ex):
    i = 4
    length = len(ex)
    while i < length:
        if (
            ex[i - 4] == "["
            and ex[i - 3].isnumeric()
            and ex[i - 2] == ","
            and ex[i - 1].isnumeric()
        ):
            ex[i - 4] = str(3 * int(ex[i - 3]) + 2 * int(ex[i - 1]))
            ex = ex[: i - 3] + ex[i + 1 :]
            i = i - 4
            length = len(ex)
        i = i + 1
    return ex


if __name__ == "__main__":
    data = open(
        "/home/jamez-amd-ubuntu/gists/input/day18_ex.in", "r", encoding="utf-8"
    ).read()
    max = 0
    dataSet = data.split()
    for i in range(len(dataSet)):
        for j in range(len(dataSet)):
            if i == j:
                continue
            mag = int(magnitude(reduceSnailfish([dataSet[i], dataSet[j]]))[0])
            if max < mag:
                max = mag
    print(max)
