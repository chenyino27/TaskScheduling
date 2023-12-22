<a name="readme-top"></a>

<h3 align="center">Task Scheduling</h3>

  <p align="center">
    NU EECE 7205 Project #2
    <br />
    <a href="https://github.com/chenyino27/TaskScheduling"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/chenyino27/TaskScheduling">View Demo</a>
    ·
    <a href="https://github.com/chenyino27/TaskScheduling/issues">Report Bug</a>
    ·
    <a href="https://github.com/chenyino27/TaskScheduling/issues">Request Feature</a>
  </p>
</div>


<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Project Construction Procedure</a>
      <ul>
        <li><a href="#prerequisites">Step I</a></li>
        <li><a href="#installation">Step II</a></li>
      </ul>
    </li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

This is a course project of EECE 7205 at Northeastern University. Background paper: [Energy and Performance-Aware Task Scheduling in a Mobile Cloud Computing
Environment](https://ieeexplore.ieee.org/abstract/document/6973741).

<p align="right">(<a href="#readme-top">back to top</a>)</p>



### Built With

* [![C++][isocpp.org]][Next-url]
* [![Xcode][developer.apple.com/documentation/xcode]][Xcode-url]


<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- PROJECT CONSTRUCTION PROCEDURE -->
## Project Construction Procedure

This project primarily addresses the task scheduling issues in multi-core processor systems. The initial scheduling algorithm arranges tasks based on minimizing time consumption. Subsequently, a novel task scheduling algorithm is proposed, which takes into full consideration the power consumption requirements of tasks. By judiciously allocating tasks to multiple cores, this algorithm aims to reduce the overall power consumption.

### Step I: Initial Scheduling Algorithm

1. Initial comparison. Evaluate the runtime of each task on both individual cores and the cloud. If the shortest runtime occurs on the cloud, designate that task for cloud execution.
2. Priority determination.
3. Establish the initial schedule. Arrange tasks in reverse order of priority and sequentially allocate each task to a schedule based on its priority.

### Step II: Task Migration Algorithm

Building upon the initial schedule, attempts are made to transfer tasks originally not designated for cloud execution to the other two cores and the cloud. This is achieved by employing a kernel algorithm to reorganize the schedule. Continuously comparing the time and energy consumption of the new and old schedules, the initial schedule undergoes iterative transformations into a new schedule with reduced power consumption.


<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- LICENSE -->
## License

MIT License

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- CONTACT -->
## Contact

Chenyi Xu - xu.chenyi@northeastern.edu

Project Link: [https://github.com/chenyino27/TaskScheduling](https://github.com/chenyino27/TaskScheduling)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

