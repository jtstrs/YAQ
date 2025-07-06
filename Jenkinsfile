pipeline {
    agent any
    stages {
        stage('Build Application') {
            steps {
                echo 'Building Application...'
                sh 'docker build --no-cache --progress=plain -t yaq-app .'
            }
        }

        stage('Run Tests') {
            steps {
                echo 'Running Tests...'
                sh 'docker run -it --rm yaq-app tests/YAQ_tests'
            }
        }
    }
}